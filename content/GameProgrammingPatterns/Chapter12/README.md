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
        move(0, 0, 20);
        playSound(SOUND_SPROING);
        spawnParticles(PARTICLE_DUST, 10);
    }
};
```
