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
* 该模式将"类型"的定义从严格生硬的代码语言转移到灵活却弱化了行为的内存对象中。灵活性是好的，但是把类型移动到数据里仍有所失。
* C++ 虚函数表就是通过类型对象实现，C++类是类型对象模式在C上的应用，由编译器自动处理。
* 通过类型对象去定义类型相关的数据非常容易，但是定义类型相关的行为却很难。如假设不同的怪物种类需要采用不同的 AI 算法。有两种方法解决:
    - 创建一个固定的预定义行为集合，让类型对象中的数据从中任选其一。
    - 在数据中定义行为，解释器模式和字节码模式都可以编译代表行为的对象。
```
class Breed {
public:
    Breed(int health, const char* attack) :
        health_(health),
        attack_(attack) {}
    int getHealth() { return health_; }
    const char* getAttach() { return attach_; }

private:
    int health; // 初始血量
    const char* attach_;
};

class Monster {
public:
    Monster(Breed& breed) 
        : health_(breed.getHealth()) ,
        , breed_(breed)
        { }
    
    const char* getAttack() {
        return breed_.getAttack();
    }

private:
    // 当前血量
    int health_;
    Breed& breed_;
}
```
* 面向对象的思想是调用类自身的构造函数，由它负责为我们创建新的实例。
```
class Breed {
public:
    // 构造器工厂方法
    Monster* newMonster() {
        return new Monster(*this);
    }

    // 之前的 Breed 代码
};

class Monster {
    friend class Breed;

public:
    const char* getAttack() {
        return breed_.getAttack();
    }

private:
    Monster(Breed& breed)
        : health_(breed_.getHealth()),
        breed_(breed)
        {}
    
    int health_; // 当前血量
    Breed breed_;
};

Monster* monster = someBreed.newMonster();
```
* 在上面的代码中，创建一个对象分为两步: 分配内存和初始化。分配内存在 newMonster() 函数内，创建对象之前，初始化在Monster构造函数内。
* 通常分配内存会有加载图像，初始化怪物AI等其他设定工作，newMonster() 函数能在控制权被移交至初始化函数之前，从一个池或者自定义堆栈里获取内存。把此逻辑放进唯一能创建怪物的Breed里，就保证了所有的怪物都由我们预想的内存管理体系经手。
* 通过继承共享数据, 例如我们要调整30多个巨魔族种类, 让他们变得更强一点.
```
class Breed {
public:
    Breed(Breed* parent, int health, const char* attack)
        : parent_(parent)
        , health_(health)
        , attack_(attack)
        {}
    
    int getHealth();
    const char* getAttack();

private:
    Breed* parent_;
    int heath_; // 初始血量
    const char* attack_;
};

int Breed::getHealth() {
    if(health_ != 0 || parent_ == NULL) {
        return health_;
    }

    return parent_.getHealth();
}

const char* Breed::getAttack() {
    if(attack_ != NULL || parent_ == NULL) {
        return attack_;
    }

    return parent_.getAttack();
}
```
* 子种族需要明确哪些特性从父类继承, 哪些特性由自己重写和特化. 如子种族只继承基种族中的非零生命值以及非NULL的攻击字符串.
* 如果能够保证基类型的属性在运行时不会变化, 一个更快的解决方案是在构造时采用继承. 即"复制"代理.
```
Breed(Breed* parent, int health, const char* attack)
    : parent_(parent)
    , health_(health)
    , attack_(attack)
{
    if(parent != NULL) {
        if(health == 0) health_ = parent->getHealth();
        
        if(attack == NULL) {
            attack_ = parent->getAttack();
        }
    }
}

int getHealth() {
    return health_;
}

const char* getAttack() {
    return attack_;
}
```
* 持有类型对象的类可以有选择地重写类型对象地行为。
```
const char* Monster::getAttack() {
    if (health_ < LOW_HEALTH) {
        return "The monster flails weakly.";
    }

    return breed_.getAttack();
}
```
* 这个模式所围绕地高级问题是如何在不同对象之间共享数据。从另一个不同角度尝试解决这个问题的是原型模式。
* 类型对象与享元模式很接近。享元模式倾向于节约内存，类型对象倾向于组织性和灵活性。
* 该模式与状态模式也有诸多相似性，都把对象的部分定义工作交给另一个代理对象实现。在类型对象中，我们通常代理的对象是：宽泛地被描述对象的静态数据。在状态模式中，我们代理的是对象当前的状态，即描述对象当前配置的临时数据。
