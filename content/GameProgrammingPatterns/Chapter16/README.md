* 服务定位器模式的简单介绍: 将一个服务的"是什么"(具体实现类型)和"在什么地方"(我们如何得到它的实例)与需要使用这个服务的代码解耦了. 下面是未解偶的代码
```
// 使用一个类的静态方法访问
AudioSystem::playSound(VERY_LOUD_BANG);

// 或者使用单例模式访问
AudioSystem::instance()->playSound(VERY_LOUD_BANG);
```
* 服务定位器模式:
    - 一个服务类为一系列操作定义了一个抽象的接口
    - 一个具体的服务提供器实现这个接口
    - 一个单独的服务定位器通过查找一个合适的提供器来提供这个服务的访问, 同时屏蔽了提供器的具体类型和定位这个服务的过程.
* 何时使用服务定位器: 谨慎使用.
* 手动将一个对象传来传去显得毫无理由或者使得代码难以阅读, 如日志系统、渲染代码的参数、内存管理系统、音频设备参数等. 将它们传递10层函数以便让一个底层的函数能够访问, 显得没有必要.
* 服务定位器的关键困难在于, 它要有所依赖(连接两份代码),并且在运行时才连接起来.
* 服务器定位器是全局可访问的, 在任何时候都能正确工作. 如果该类只能在帧更新时工作,不能在帧渲染时工作,则该类不能当作服务.
* 例子, 以下为音频服务暴露的接口
```
class Audio {
public:
    virtual ~Audio() {}
    virtual void playSound(int soundId) = 0;
    virtual void stopSound(int soundId) = 0;
    virtual void stopAllSounds() = 0;
};
```
* 服务提供器
```
class ConsoleAudio : public Audio {
public:
    virtual void playSound(int SoundId) {
        // 使用控制台音频 API 播放声音
    }
    virtual void stopSound(int SoundId) {
        // 使用控制台音频 API 停止声音
    }
    virtual void stopAllSounds() {
        // 使用控制台音频 API 停止所有声音
    }
};
```
* 服务定位器
```
class Locator {
public:
    static Audio* getAudio() { return service_; }

    static void provide(Audio* service) {
        service_ = service;
    }

private:
    static Audio* service_;
};
```
* 上面的代码 getAudio() 负责定位工作, 能在代码的任何地方调用它, 返回一个 Audio 服务的实例供我们使用
```
Audio* audio = Locator::getAudio();
audio->playSound(VERY_LOUD_BANG);
```
* 启动时注册一个服务提供器
```
ConsoleAudio* audio = new ConsoleAudio;
Locator::provide(audio);
```
* 以下好处:
    - 调用 playSound() 的代码不知道 ConsoleAudio 的具体实现
    - 调用 playSound() 的代码只知道 Audio 的抽象接口
    - 定位器本身和具体服务器之间没有耦合,只有初始化代码部分进行关联.
    - Audio 接口在绝大多数地方并不知道自己正在被访问.
* 使用一个空服务避免返回null指针的问题
```
class NullAudio : public Audio {
public:
    virtual void playSound(int SoundId) {
    }
    virtual void stopSound(int SoundId) {
    }
    virtual void stopAllSounds() {
    }
};

class Locator {
public:
    static void initialize() {
        serveice_ = &nullService_;
    }
    static Audio* getAudio() { return service_; }

    static void provide(Audio* service) {
        if(service == NULL) service = &nullService_;

        service_ = service;
    }

private:
    static Audio* service_;
    static NullAudio nullService_;
};
```
* 使用装饰器模式暴露不同系统的条件日志
```
class LoggedAudio : public Audio {
public:
    LoggedAudio(Audio &wrapped) : wrapped_(wrapped) {}
    virtual void playSound(int soundId) {
        log("play sound");
        wrapped_.playSound(soundId);
    }

    virtual void stopSound(int soundId) {
        log("play sound");
        wrapped_.stopSound(soundId);
    }

    virtual void playSound(int soundId) {
        log("stop sound");
        wrapped_.playSound(soundId);
    }

    virtual void stopAllSounds() {
        log("stop all sounds");
        wrapped_.stopAllSounds();
    }
private:
    void log(const char* message) {
        // 记录日志消息的代码
    }

    Audio &wrapped_;
};
```
* 如果要开启音频日志, 可以这样调用代码:
```
void enableAudioLogging() {
    // 装饰已有的服务
    Audio* service = new LoggedAudio(Locator::getAudio());
    Locator::provide(service);
}
```
* 服务器如何被定位
    - 外部代码注册, 如上面的例子
    - 编译器绑定
    - 通过配置文件
* 服务器不能定位,即为空时可以有以下处理方式:
    - 使用者决定如何处理空服务
    - 使用断言, 即在 getAudio() 里面使用 assert(service != NULL) 断言
    - 返回一个空服务
* 还可以限制服务的作用域范围
```
class Base {
    // 用于定位服务和设置服务的方法

protected:
    // 只有派生类可以使用服务
    static Audio& getAudio() { return *service_; }

private:
    static Audio* service_;
}
```
* 如果服务被限制在游戏的一个单独域中, 那么就把服务的作用域限制在类中. 如获取网络访问的服务就可以被限制在联网的类中.更广泛使用的服务,如日志服务就应该是全局的.
* Unity 框架把这个模式和组件模式结合起来,并使用在了 GetComponent() 方法中.