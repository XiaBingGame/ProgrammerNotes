# 第19章 对象池
* 使用固定的对象池重用对象, 取代单独地分配和释放对象, 以此来达到提升性能和优化内存使用的目的.
* 定义一个保持着可重用对象集合的对象池类. 其中的每个对象支持对其"使用(in use)"状态的访问, 以确定这一对象目前是否"存活(alive)".
* 在对象池初始化时, 它预先创建整个对象的集合(通常为一块连续堆区域), 并将它们都置为"未使用(not in use)"状态.
* 当你想要创建一个新对象时, 就向对象池请求, 它将搜索到一个可用的对象, 将其初始化为"使用中(in use)"状态并返回给你, 当不再使用时, 它将被置回"未使用(not in use)"状态. 使用该方法, 对象便可以在无需进行内存或其他资源分配的情况下进行任意的创建和销毁.
* 使用情境:
    - 需要频繁地创建和销毁对象时
    - 对象大小一致时
    - 在堆上进行对象内存分配较慢或会产生内存碎片时
    - 每个对象封装着获取代价昂贵且可重用的资源, 如数据库, 网络的连接.
* 使用须知:
    - 对象池的大小需要权衡, 过小不够用, 过大则浪费
    - 申请重用某个对象时, 由于对象池用光申请失败, 有以下几个常见对策:
        - 阻止其发生, 如为了极端情况申请足够大的对象池大小, 或针对不同的场景将对象池调整至不同尺寸.
        - 不创建对象, 如粒子系统, 少几个粒子也看不出来.
        - 强行清理现存对象, 例如音效对象, 清理掉不引人注意的音效部分.
        - 运行时调整对象池的大小, 或增设一个二级的溢出池.
    - 如果对象大小不一时, 为了避免浪费, 可以考虑根据对象的尺寸将对象池划分为多个大小不同的池.
* 粒子代码
```
class Particle {
public:
    Particle() : framesLeft_(0) {}

    void init(double x, double y, 
        double xVel, double yVel, int lifetime);
    void animate();
    bool inUse() const { return framesLeft_ > 0; }

private:
    int framesLeft_;
    double x_, y_;
    double xVel_, yVel_;
};

void Particle::init(double x, double y, 
        double xVel, double yVel, int lifetime) {
    x_ = x;
    y_ = y;
    xVel_ = xVel;
    yVel_ = yVel;
    frameLeft_ = lifetime;
}

void Particle::animate() {
    if(!inUse()) return;

    frameLeft_--;
    x_ += xVel_;
    y_ += yVel_;
}

class ParticlePool {
public:
    void create(double x, double y, 
        double xVel, double yVel, int lifetime);

    void animate();

private:
    static const int POOL_SIZE = 100;
    Particle particles_[POOL_SIZE];
}

void ParticlePool::animate() {
    for(int i = 0; i < POOL_SIZE; i++) {
        particles_[i].animate();
    }
}

void ParticlePool::create(double x, double y, 
        double xVel, double yVel, int lifetime) {
    for (int i = 0; i < POOL_SIZE; i++) {
        if(!partiles_[i].inUse()) {
            particles_[i].init(x, y, xVel, yVel, lifetime);
            return;
        }
    }
}
```
* 使用空闲表解决查找对象池可用对象的问题
```
class Particle {
public:
    // 之前的成员
    Particle* getNext() const { return state_.next; }
    void setNext(Particle* next) { state_.next = next; }

private:
    int frameLeft_;

    union {
        // 当被使用时使用该成员
        struct {
            double x, y, xVel, yVel;
        } live;

        // 当不被使用时使用该成员
        Particle* next;
    } state_;
};

class ParticlePool { 
// 之前的成员
private:
    ParticelPool* firstAvailable_;
};

PariticlePool::ParticlePool() {
    firstAvailable_ = &particles_[0];

    for(int i = 0; i < POOL_SIZE - 1; i++) {
        particles_[i].setNext(&particles_[i + 1]);
    }

    particles_[POOL_SIZE - 1].setNext(NULL);
}

void ParticlePool::create(double x, double y, 
        double xVel, double yVel, int lifetime) {
    assert(firstAvailable_ != NULL);

    Particle* newParticle = firstAvailable_;
    firstAvailable_ = newParticle->getNext();
    newParticle->init(x, y, xVel, yVel, lifetime);
}

bool Particle::animate() {
    if (!inUse()) return false;

    framesLeft--;
    x_ += xVel_;
    y_ += yVel_;

    return framesLeft_ == 0;
}

void ParticlePool::animate() {
    for(int i = 0; i < POOL_SIZE; i++) {
        if(particles_[i].animate()) {
            particles_[i].setNext(firstAvailable_);
            firstAvailable_ = particles_[i];
        }
    }
}
```
* 解决对象和对象池耦合问题: 
    - 对象池类作为对象的友元类, 对象自身的构造函数私有化.
    - 对象可以提供 inUse() 方法来检查自己是否被使用的状态, 避免对象池使用额外的空间来存储那些"使用中"的标志位.
    - 如果对象独立于对象池, 建立通用的对象池, 则使用独立的空间跟踪对象的"使用中"状态
```
// 用模板实现通用的对象池
template <class TObject>
class GenericPool {
private:
    static const int POOL_SIZE = 100;

    TObject pool_[POOL_SIZE];
    bool inUse_[POOL_SIZE];
};
```
* 对象池负责初始化
```
class ParticlePool {
public:
    void create(double x, double y) {
        // ...
    }

    void create(double x, double y, double angle) {
        // ...
    }

    void create(double x, double y, 
        double xVel, double yVel) {
        // ...
    }
};
```
- 对象自己负责初始化
```
class Particle {
    void init(double x, double y);
    void init(double x, double y, double angle);
    void init(double x, double y, double xVel, double yVel);
};

class ParticlePool {
public:
    Particle* create() {
        // ...
    }
};

ParticlePool pool;
pool.create()->init(1, 2);
pool.create()->init(1, 2, 0.3);
pool.create()->init(1, 2, 3.3, 4.4);

// 或者判断下是否成功创建
Particle* particle = pool.create();
if(particle != NULL) particle->init(1, 2);
```