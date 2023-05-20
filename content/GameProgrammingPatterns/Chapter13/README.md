* 怪物基类
```
class Monster {
public:
    virtual ~Monster() {}
    virtual const char* getAttack() = 0;

protected:
    Monster(int startingHealth)
        : health_(startingHealth) {}
    
private:
    int health_; // 当前血量
};
```
* 两个子种族类
```
class Dragon : public Monster {
public:
    Dragon() : Monster(230) {}

    virtual const char* getAttack() {
        return "The dragon breathes fire!";
    }
};

class Troll : public Monster {
public:
    Troll() : Monster(48) {}

    virtual const char* getAttack() {
        return "The troll clubs you!";
    }
};
```
* 需要一种无需重新编译整个游戏, 就能修改种族数值的能力.如果设计师在无需程序员介入的情况下就能创建并调整种族属性,那就更好了.
* 如果每个怪物种族都创建一个类,则怪物这个继承树就会变得非常庞大,其添加新的种族意味着添加新的代码.
* 只创建两个类, 一个 Monster 类, 一个 Breed(种类)类.实现每个怪物都有"has a"种类. Breed类包含了同一种族的所有怪物之间共享的信息:初始生命值和攻击字符串.
* 这个模式的强大之处在于,不使代码库复杂化的情况下添加新的类型. 通过实例化更多的 Breed 实例来创建数以千计的种族, 甚至可以通过配置文件的数据来初始化种族.
* 创建一个类型对象类, 并创建其一个实例, 引用同一个实例对于持有该实例的类来说, 表现出"同类"的性状
* 使用情境: 当你需要定义一系列不同"种类"的东西, 但又不想把那些种类硬编码进你的类型系统时, 本模式都适用.
    - 你不知道将来会有什么类型.
    - 你需要在不重新编译或修改代码的情况下,修改或添加新的类型.