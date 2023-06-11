* 战斗模块的代码在每次消灭一个敌人给事件队列添加一个“敌人死亡”的事件。然后新手引导模块向事件队列注册自身，并向其声明希望接收“敌人死亡”事件。从而实现敌人死亡时触发新手引导的功能。
* 假设有一个音效引擎
```
class Audio {
public:
    // 根据 SoundId 加载对应的声音资源, 提供可用的声道并开始将它播放出来.
    static void playSound(SoundId id, int volume);
};

void Audio::playSound(SoundId id, int volume) {
    ResourceId resource = loadSound(id);
    int channel = findOpenChannel();
    if(channel == -1) return;
    startSound(resource, channel, volume);
}
```
* 假设游戏有个菜单, 选择菜单时播放音效
```
class Menu {
public:
    void onSelect(int index) {
        Audio::playSound(SOUND_BLOOP, VOL_MAX);
    }
};
```
* 上面的代码有几个问题:
    - 在音效引擎完全处理完播放请求前, Audio API 的调用一直阻塞调用者.
    - 多个怪物的声音同时播放,会叠加它们的声波,产生问题
    - 不能批量处理请求,不同声音在不同线程播放时,有资源冲突问题.
* 如果你只想对一条消息的发送者和接收者进行解耦, 观察者模式和命令模式都能以更低的复杂度满足你.
* 使用方法
    - 将中心队列事件创建成一个全局变量，但其容易产生互相依赖
    - 事件可能需要保存事件触发时的周边状态，如击杀怪物时的周围怪物数量和难度。
* 使用结构体存储待处理的请求
```
struct PlayMessage {
    SoundId id;
    int volume;
};
```
* 使用数组存储一系列同结构事物
```
class Audio {
public:
    static void init() { numPending_ = 0; }

    void playSound(SoundId id, int volume);

    // 可能在游戏主循环中调用，也可能在一个专用的声音线程里调用
    static void update() {
        for (int i = 0; i < numPending_; i++) {
            ResourceId resource = loadSound(pending_[i].id);
            int channel = findOpenChannel();
            if (channel == -1) return;
            startSound(resource, channel, pending_[i].volume);
        }

        numPending_ = 0;
    }
    // 其他内容...
private:
    static const int MAX_PENDING = 16;

    static PlayMessage pending_[MAX_PENDING];
    static int numPending_;
};

void Audio::playSound(SoundId id, int volume) {
    assert(numPending_ < MAX_PENDING);

    pending_[numPending_].id = id;
    pending_[numPending_].volume = volume;
    numPending_++;
}
```
* 上面的代码需要一次性处理全部请求
* 环状缓冲区
    - 队列的 head(队头) 是请求被读取的地方
    - 队列的 tail(队尾) 是下一个队列请求写入的位置
```
class Audio {
public:
    static void init() {
        head_ = 0;
        tail_ = 0;
    }

    // 方法...
private:
    static int head_;
    static int tail_;

    // 数组
};

void Audio::playSound(SoundId id, int volume) {
    assert((tail_+1) % MAX_PENDING != head_);

    pending_[tail_].id = id;
    pending_[tail_].volume = volume;
    tail_ = (tail_ + 1) % MAX_PENDING;
}

void Audio::update() {
    if(head_ == tail_) return;

    ResourceId resource = loadSound(pending_[head_].id);
    int channel = findOpenChannel();
    if (channel == -1) return;
    startSound(resource, channel, pending_[head_].volume);    
    head_ = (head_ + 1) % MAX_PENDING;
}
```
* 汇总请求, 如果播放两个相同的音频, 则使用最大的那个音量
```
void Audio::playSound(SoundId id, int volume) {
    for (int i = head_; i != tail_; i = (i+1) % MAX_PENDING) {
        if(pending_[i].id == id) {
            pending_[i].volume = max(volume, pending_[i].volume);

            return;
        }
    }

    // 之前的代码
}
```
* 上面的代码也可以在 update() 内进行汇总, 避免调用上面的函数等待时间过久.
* 队列内容的类型有事件和消息两种。
    - 如果是事件，则可能允许多个监听器，有更广的全局可见性。
    - 如果是消息，则可能表示“期望”发生在“将来”的行为，如“播放音乐”。可能只有单一的监听器，如专门向音频 API 请求播放声音。
* 队列的读取：
    - 单播队列: 队列是一个类 API 本身的一部分, 例如声音例子, 调用者(发送者)只能调用 playSound() 接口.
    - 广播队列: 当一件事件进来时,可能有多个监听器.其中事件可以被删除,被处理,被过滤.
    - 工作队列: 同样有多个监听器,但队列中的每一项只会被投递到一个监听器中, 而不是多个监听器中,这是一种对于并发线程支持不好的系统中常见的工作分配模式.
* 队列的写入
    - 一个写入者, 可能有多个读取者. 默认都知道谁是发送者.
    - 多个写入者, 如上面的音频引擎例子. 如果需要知道谁是发送者, 通常需要传递发送者在事件本身的引用.
* 队列中对象的生命周期
    - 转移所有权: 当消息入队时,队列拥有所有权,发送者不再拥有它.当消息处理时,接收者取走所有权并负责释放它.
    - 共享所有权: 通过引用计数控制消息的释放
    - 队列拥有它: 消息总是存在于队列中, 发送者会从队列中请求一个新的消息. 队列返回一个已经存在于队列内存的消息引用,接着发送者填充该引用所指向的消息.消息处理时,接收者参考队列中相同消息的操作.
* 一个有限状态机, 类似于 GoF 的状态模式, 需要一个输入流, 如果你想要异步地响应它们, 把它们入列就好. 当你有一堆状态机互相发送消息的时候, 每个状态机都有一个小的队列等待输入, 于是你就重新发明出了计算角色模型.
* Go 编程语言内置的"通道"类型, 本质上就是一个事件队列或者消息队列.