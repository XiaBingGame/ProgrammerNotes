* 提到两块代码是“解耦”时， 我们指的是某块代码中的变化通常不会影响到另一块代码
* 允许一个单一的实体跨越多个不同域而不会导致耦合
* 我们可以将通信限制在那些需要交互的组件之间而不是将它们全部放到一起。
* "致命的菱形多继承": 发生在对同一基类有多条路径的多重继承的类层次结构中。
* 假设场景有多个游戏对象，Zone 需要实现物理碰撞，Decoration 需要实现图形渲染，Prop (道具)需要实现物理碰撞喝图形渲染。因此包含一个简单的 GameObject 基类和两个组件: 物理组件(PhysicsComponent)和图像组件(GraphicsComponent), Decoration 包含图像组件不包含物理组件, Zone 包含物理组件不包含图像组件, Prop 包含图像组件也包含物理组件.
* 当如下条件成立时，可以使用组件模式：
    - 你有一个涉及多个域的类，但是你希望让这些域保持相互解耦。
    - 一个类越来越庞大，越来越难以开发
    - 希望定义许多共享不同能力的对象, 但采用继承的办法却无法令你精确地重用代码。
* 不同组件的通信变得更具挑战性，而且对它们所占用内存的管理将更复杂。
* 一个庞大的类
```
class Bjorn {
public:
    Bjorn() : velocity_(0), x_(0), y_(0) {}

    void update(World& world, Graphics& graphics);
private:
    static const int WALK_ACCELERATION = 1;

    int velocity_;
    int x_;
    int y_;
    Volume volume_;

    Sprite spriteStand_;
    Sprite spriteWalkLeft_;
    Sprite spriteWalkRight_;
};

void Bjorn::update(World& world, Graphics& graphics) {
    // 用户的输入影响英雄的速度
    switch (Controller::getJoystickDirection()) {
    case DIR_LEFT:
        velocity_ -= WALK_ACCELERATION;
        break;

    case DIR_RIGHT:
        velocity_ += WALK_ACCELERATION;
        break;
    }

    // 通过速度修改位置
    x_ += velocity_;
    world.resolveCollision(volume_, x_, y_, velocity_);

    // 绘制合适的目标
    Sprite* sprite = &spriteStand_;
    if (velocity_ < 0) sprite = &spriteWalkLeft_;
    else if (velocity_ > 0) sprite = &spriteWalkRight_;
    graphics.draw(*sprite, x_, y_);
}
```
* 建立一个类单独处理输入
```
class InputComponent {
public:
    void update(Bjorn& bjorn) {
        switch (Controller::getJoystickDirection()) {
        case DIR_LEFT:
            bjorn.velocity_ -= WALK_ACCELERATION;
            break;
        
        case DIR_RIGHT:
            bjorn.velocity_ += WALK_ACCELERATION;
            break;
        }
    }

private:
    static const int WALK_ACCELERATION = 1;
};

class PhysicsComponent {
public:
    void update(Bjorn& bjorn, World& world) {
        bjorn.x += bjorn.velocity;
        world.resolveCollision(volume_, bjorn.x, bjorn.y, bjorn.velocity);
    }

private:
    Volume volume_;
};

class GraphicsComponent {
    void update(Bjorn& bjorn, Graphics& graphics) {
        Sprite* sprite = &spriteStand_;
        if (bjorn.velocity_ < 0) sprite = &spriteWalkLeft_;
        else if (bjorn.velocity_ > 0) sprite = &spriteWalkRight_;
        graphics.draw(*sprite, x_, y_);
    }
private:
    Sprite spriteStand_;
    Sprite spriteWalkLeft_;
    Sprite spriteWalkRight_;
}
```
* 修改原有 Bjorn 类的 update 函数
```
class Bjorn {
public:
    int velocity_;
    int x_;
    int y_;
    Bjorn() : velocity_(0), x_(0), y_(0) {}

    void update(World& world, Graphics& graphics) {
        input_.update(*this);
        physics_.update(*this, world);
        graphics.update(*this, graphics);
    }
private:
    InputComponent input_;
    PhysicsComponent physics;
    GraphicsComponent graphics_;
};
```
* 添加新的物理组件
```
class PhysicsComponent {
public:
    void update(Bjorn& bjorn, World& world) {
        bjorn.x += bjorn.velocity;
        world.resolveCollision(volume_, bjorn.x, bjorn.y, bjorn.velocity);
    }

private:
    Volume volume_;
};
```
* 上述代码的问题在于构建 Bjorn 类时需要知道用哪个 InputComponent, 可以在创建 Bjorn 对象时再确定使用哪个 InputComponent
```
class InputComponent {
public:
    virtual ~InputComponent() {}
    virtual void update(Bjorn& bjorn) = 0;
};

class PlayerInputComponent : public InputComponent {
public:
    void update(Bjorn& bjorn) {
        switch (Controller::getJoystickDirection()) {
        case DIR_LEFT:
            bjorn.velocity_ -= WALK_ACCELERATION;
            break;
        
        case DIR_RIGHT:
            bjorn.velocity_ += WALK_ACCELERATION;
            break;
        }
    } 

private:
    static const int WALK_ACCELERATION = 1;
};

class Bjorn {
public:
    int velocity_;
    int x_;
    int y_;
    Bjorn(InputComponent* input) : input_(input), velocity_(0), x_(0), y_(0) {}

    void update(World& world, Graphics& graphics) {
        input_->update(*this);
        physics_.update(*this, world);
        graphics.update(*this, graphics);
    }
private:
    InputComponent* input_;
    PhysicsComponent physics;
    GraphicsComponent graphics_;
};

Bjorn* bjorn = new Bjorn(new PlayerInputComponent());
```