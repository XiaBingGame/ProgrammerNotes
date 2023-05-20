* 使用基类提供的操作集合来定义子类中的行为.
* 假设实现一个超能力基类 SuperPower 类, 而后有大量的超能力派生自该类实现不同的超能力, 如果不使用沙盒则产生如下问题:
    - 会产生大量冗余代码, 如冰冻射线、火焰射线、芥末涉嫌时，你会发现它们在实现上都极为相似。
    - 游戏引擎的每个部分都将与大量的子类产生耦合
    - 当外部系统需要改变的时候, 子类的代码会被随机的改变。这样引擎的图形、音效、UI程序员可能都要成为逻辑程序员
    - 定义所有子类都遵循的不变量是困难的，例如几百个子类都直接调用音效引擎的话，就会很难实现。
* 给每个设计 SuperPower 子类的游戏逻辑程序员一系列可用的基本操作函数. 如 playSound() 函数和 spawnParticles() 方法。 这些方法设置为基类受保护的方法
* 子类沙盒方法，实现一个新的 power 所要做的：
    - 创建一个继承自 Superpower 的新类
    - 覆写沙盒函数 activate()
    - 通过调用 Superpower 提供的保护函数来实现新类方法的函数体。
* 当我们发现在子类中存在大量重复代码时，我们就会把它上移到基类作为一个新的可用基本操作
* 把耦合制约于一处来解决耦合问题，基类最终将与不同的游戏系统耦合，但我们的上百个子类则不会，它们仅与基类耦合
* 沙盒模式: 一个基类定义了一个抽象的沙盒方法和一些预定义的操作集合。通过将它们设置为受保护的状态以确保它们仅供子类使用。每个派生出的沙盒子类根据父类提供的操作来实现沙盒函数。
* 子类沙盒的使用情境
    - 有一个带有大量子类的基类
    - 基类能够提供所有子类可能需要执行的操作集合
    - 子类之间有重叠的代码，你希望在它们之间更简便地共享代码
    - 希望继承类与程序其他代码之间的耦合最小化
```
class Superpower {
public:
    virtual ~Superpower();

protected:
    // 沙盒函数
    virtual void activate() = 0;

    void move(double x, double y, double z) {
        // 这里编写代码
    }

    void playSound(SoundId sound) {
        // 这里编写代码
    }

    void spawnParticles(ParticleType type, int count) {
        // 这里编写代码
    }

    double getHeroX() { /* 代码写在这里 */ }
    double getHeroY() { /* 代码写在这里 */ }
    double getHeroZ() { /* 代码写在这里 */ }
};

class SkyLauch : public Superpower {
protected:
    virtual void activate() {
        if(getHeroZ() == 0) {
            // 在地面上，所以弹向天空
            playSound(SOUND_SPROING);
            spawnParticles(PARTICLE_DUST, 10);
            move(0, 0, 20);
        }
        else if(getHeroZ() < 10.0f) {
            // 地面附近，做一个二度起跳
            playSound(SOUND_SWOOP);
            move(0, 0, getHeroZ() + 20);
        }
        else {
            // 在空中， 做一个俯冲攻击
            playSound(SOUND_DIVE);
            spawnParticles(PARTICLE_SPARKLES, 1);
            move(0, 0, -getHeroZ());
        }
    }
};
```
* 子类沙盒方法有两个极端情况:
    - 基类不提供任何操作, 仅包含一个沙盒方法. 子类调用基类之外的系统, 和基类之外的系统进行了深度耦合
    - 基类为子类提供了所需的所有操作, 子类仅与基类耦合, 不调用任何外部系统.
* 两个极端情况之间有一个很宽阔的中间地带, 在这个空间里, 一些操作由基类提供, 另外一些则通过定义它的的外部系统直接访问. 基类提供越多的操作,子类与外部系统的耦合越少,但基类与外部耦合的程度就越高,它去掉了继承类的耦合, 但这是通过把耦合聚集到基类自身来实现的.
* 通过把耦合提取进一个操作方法,你把它们聚集到了一个地方---基类,但是你越是这么做,基类就变得越大且越来越难以维护.
* 经验法则:
    - 如果所提供的操作仅仅被一个或者少数的子类所使用,那么不必将它加入基类. 这只会给基类增加复杂度,同时影响每个子类,而仅有少数子类从中受益.
    - 当你在游戏的其他模块进行某个方法调用时, 如果它不修改任何状态,那么它不具备侵入性. 它仍然产生了耦合, 但这是个"安全"的耦合.因为它不带来任何破坏.
    - 如果某个方法调用确实改变了状态, 则将与代码库产生更大的耦合, 你需要对这些耦合更上心.因为这些方法更适合由更可视化的基类提供.
    - 如果提供的操作,其实现仅仅是对一些外部系统调用的二次封装, 那么它并没有带来多少价值,这种情况直接调用外部系统更为简单.
    - 有些二次封装仅仅是基类不想向子类暴露状态. 如下面的方法
```
// 子类无法接触到 soundEngine_
void playSound(SoundId sound) {
    soundEngine_.play(sound);
}
```
* 基类可能过于臃肿, 可以通过转移一些函数到其他类中来缓解这种情况
```
class Superpower {
protected:
    void playSound(SoundId sound) { /* 编码... */ }
    void stopSound(SoundId sound) { /* 编码... */ }
    void setVolume(SoundId sound) { /* 编码... */ }
    // 一些方法和其他操作
};
```
* 代码修改如下
```
class SoundPlayer {
    void playSound(SoundId sound) { /* 编码... */ }
    void stopSound(SoundId sound) { /* 编码... */ }
    void setVolume(SoundId sound) { /* 编码... */ }
};

// Superpower 提供这个对象的访问
class Superpower {
protected:
    SoundPlayer& getSoundPlayer() {
        return soundPlayer_;
    }

private:
    SoundPlayer* soundPlayer_;
};
```
* 基类希望封装一些数据对子类保持隐藏, 例如基类需要粒子系统实现特效
```
class Superpower {
public:
    Superpower(ParticleSystem* particles)
        : particles_(particles) {}
    // 沙盒方法和其他操作...

private:
    ParticleSystem* paritcles_;
};

class SkyLaunch : public Superpower {
public:
    SkyLaunch(ParticleSystem* particles) : Superpower(particles) {}
};
```
* 这样基类向每一个子类暴露了粒子系统参数. 如果基类需要添加另外一个状态,每个派生类都需要修改构造函数来传递该状态.
* 可以进行分段初始化, 构造函数不带参数,仅负责创建对象,然后再基类中定义其他函数传递其所需要的其他数据.
```
Superpower* power = new SkyLaunch();
power->init(particles);
```
* 封装整个创建过程到单个函数确保创建对象后会调用 init() 方法传递数据.
```
Superpower* createSkyLaunch(ParticleSystem* particles) {
    Superpower* power = new SkyLaunch();
    power->init(particles);
    return power;
}
```
* 假设粒子系统是一个单例,每个power实例共享相同的粒子系统,则声明为基类的静态私有成员, 这样只需要针对整个游戏对Superpower类初始化一次,而不是为每个实例都初始化一次.
```
class Superpower {
public:
    static void init(ParticleSystem* particles) {
        particles_ = particles;
    }

private:
    static ParticleSystem* particles_;
};
```
* 在基类中使用服务器定位器获取需要的数据, 而不是让周围代码进行初始化.
```
class Superpower {
protected:
    void spawnParticles(ParticleType type, int count) {
        ParticelSystem& particles = Locator::getParticles();
        particles.spawn(type, count);
    }
};
```