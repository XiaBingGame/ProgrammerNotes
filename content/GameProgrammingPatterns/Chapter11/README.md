* 例如实现游戏中的法术，不适用程序里用代码写，因为这样不易于修改。
* 将其写成单独的“数据”，在单独的数据文件中定义行为，游戏引擎以某种方式加载并“执行”它们.
* 定义一个所有表达式都必须实现的基础接口:
```
class Expression {
public:
    virtual ~Expression();
    virtual double evaluate() = 0;
};
```
* 为语言中的每个语法定义类来实现这个接口
```
class NumberExpression : public Expression {
public:
    NumberExpression(double value) : value_(value) {}

    virtual double evaluate() { return value_; }

private:
    double value_;
};

class AdditionExpression : public Expression {
public:
    AdditionExpression(Expression* left, Expression* right) 
        : left_(left) 
        , right_(right)
        {}

    virtual double evaluate() { 
        double left = left_->evaluate();
        double right = right_->evaluate();

        return left + right;
    }

private:
    Expression* left_;
    Expression* right_;
};
```
* 缺点：速度慢且占用内存。
* C++ 程序执行的是源程序编译生成的机器码。机器码的好处在于:
    - 高密度: 连续的二进制数据块
    - 线性：指令被打包且顺序执行
    - 底层
    - 迅速
* 实现成为虚拟机(VM)的小型模拟器, 这个虚拟机所执行的语义上的“二进制机器码”称之为字节码，具备在数据内定义对象的灵活性和易用性，同时也比解释器模式这种高级呈现方式更高效。
* 字节码模式: 指令集定义了一套可以执行的底层操作。一系列指令被编码为字节序列，虚拟机逐条执行指令栈上这些指令。通过组合指令，即可完成很多高级行为。 
* 使用情境: 仅当你的游戏需要定义大量行为, 而且实现游戏的语言出现下列情况时才应该使用:
    - 编程语言太底层了，编写起来繁琐易错。
    - 编译时间太长或工具问题，导致迭代缓慢
    - 安全性太依赖编码者
* 首先为虚拟机设计一个指令集， 可以先把它们当成是 API。
    - 绝大多数法术会改变巫师身上的某个状态，设置一组状态
```
// 设置生命值
void setHealth(int wizard, int amount);
// 设置智力
void setWisdom(int wizard, int amount);
// 设置敏捷
void setAgility(int wizard, int amount);
// 播放声音
void playSound(int soundId);
// 释放粒子效果
void spawnParticles(int particleType);
```

* 法术就是一个指令集
```
enum Instruction {
    INST_SET_HEALTH      = 0x00,
    INST_SET_WISDOM      = 0x01,
    INST_SET_AGILITY     = 0x02,
    INST_PLAY_SOUND      = 0x03,
    INST_SPAWN_PARTICLES = 0x04
}
```
* 枚举值长度取一个字节，即法术代码都是一个字节列表---这就是所谓的字节码。
* 执行一条指令时, 我们首先找到对应的基础属性, 然后调用正确的API。
```
switch (instruction) 
{
case INST_SET_HEALTH:
    setHealth(0, 100);
    break;

case INST_SET_WISDOM:
    setWisdom(0, 100);
    break;

case INST_SET_AGILITY:
    setAgility(0, 100);
    break;

case INST_PLAY_SOUND:
    playSound(SOUND_BANG);
    break;

case INST_SPAWN_PARTICLES:
    spawParticles(PARTICLE_FLME);
    break;
}

```
* 实现一个简单的虚拟机
```
class VM {
public:
    void interpret(char bytecode[], int size) {
        for (int i = 0; i < size; i++) {
            char instruction = bytecode[i];
            switch (instruction) {
                // 执行各种指令码
            }
        }
    }

};
```
* 通过堆栈的方式实现虚拟机
```
class VM {
public:
    VM() : stackSize_(0) { }

    // 其他内容

private:
    void push(int value) {
        assert(stackSize_ < MAX_STACK);
        stack_[stackSize_++] = value;
    }

    int pop() {
        // 确保栈非空
        assert(stackSize_ > 0);
        return stack_[--stackSize_];
    }

    static const int MAX_STACK = 128;    
    int stackSize_;
    int stack_[MAX_STACK];
};
```
* 当某个指令需要输入参数时， 它会按照下面的方式从堆栈中弹出来
```
switch(instruction) {
    case INST_SET_HEALTH:
    {
        int amount = pop();
        int wizard = pop();
        setHealth(wizard, amount);
        break;
    }

    // 类似的代码用于 SET_WISDON 和 SET_AGILITY...
    case INST_SPAWN_PARTICLES:
    {
        playSound(pop());
        break;
    }
}
```
* 将数字塞进字节数组
```
switch (instruction) {
    case INST_LITERAL:
    {
        int value = bytecode[++i];
        push(value)；
        break;
    }
}
```
* 假设有指令集： 字面数值-> 0 -> 字面数值 -> 10 -> 治疗(释放魔法)
* 添加读取状态的指令
```
case INST_GET_HEALTH:
{
    int wizard = pop();
    push(getHealth(wizard));
    break;
}

case INST_GET_WISDOM:
    // ...

case INST_GET_AGILITY:
    // ...
```
* 加法指令的实现
```
case INST_ADD:
{
    int b = pop();
    int a = pop();
    push(a+b);
    break;
}
```
* 将玩家巫师的生命设定成他们敏捷值和智力值的平均值
```
setHealth(0, getHealth(0) + (getAigility(0) + getWisdom(0)) / 2);
```
* 指令集如下, 假设生命值为45， 敏捷为7， 智力为 11
```
LITERAL 0       [0]
LITERAL 0       [0, 0]
GET_HEALTH      [0, 45]
LITERAL 0       [0, 45, 0]
GET_AGILITY     [0, 45, 7]
LITERAL 0       [0, 45, 7, 0]
GET_WISDOM      [0, 45, 7, 11]
ADD             [0, 45, 18]
LITERAL 2       [0, 45, 18, 2]
DIVIDE          [0, 45, 9]
ADD             [0, 54]
SET_HEALTH      []
```
* 基于栈的虚拟机中，指令总是操作栈顶。基于寄存器的虚拟机也有一个堆栈，唯一的区别就是指令可以从栈的更深层次中读取输入。不像“INST_ADD”那样总是出栈操作数。在字节码中存储两个索引来表示应该从堆栈的哪个位置读取操作数。
* Lua 寄存器式虚拟机 Lua，每个指令占用 32 位，其中 6 位存储指令类型，剩下的存储参数。
* 可能需要的指令类型:
    - 外部基本操作: 位于虚拟机之外，引擎内部的，决定字节码能够表达的真正行为
    - 内部基本操作: 操作虚拟机内部的值，如字面值、算术运算符、比较运算符和操作栈的指令
    - 控制流：实现为跳转。
    - 抽象化：即可调用过程。维护另一个“返回”堆栈
* 一个功能完善的虚拟机应当支持不同的数据类型: 字符串, 对象, 列表等, 你必须决定如何在内部存储它们.
  * 单一数据类型, 如只有整数
  * 标签的一个变体. 每个值都由两部分组成, 第一部分是个标签, 一个用来标志所存储数据类型的枚举值
```
enum valueType {
    TYPE_INT,
    TYPE_DOUBLE,
    TYPE_STRING,
};
// 剩下的位根据类型值来解析
struct Value {
    ValueType type;
    union {
        int intValue;
        double doubleValue;
        char* stringValue;
    }
};
```
* 不带标签的联合体
* 多态
