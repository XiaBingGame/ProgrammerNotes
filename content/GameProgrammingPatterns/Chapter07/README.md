* 一些代码控制问题， 比如实现了按键跳跃功能， 就要判断在跳跃的时候按下跳跃按键不会继续跳跃之类的问题。
* 有限状态机
    - 你拥有一组状态， 并且可以在这组状态之间进行切换。比如：站立，跳跃，躲避和俯冲。
    - 状态机同一时刻只能处于一种状态。女主角无法同时跳跃和站立。防止同时存在两个状态是我们使用有限状态机的原因。
    - 状态机会接收一组输入或者事件，如按钮按下和释放。
    - 每一个状态有一组状态，每一个状态都关联着一个输入并指向另一个状态。
* 整个状态机可以分为：状态、输入和转换。
```
enum State {
    STATE_STANDING,
    STATE_JUMPING,
    STATE_DUCKING,
    STATE_DIVING
}
```
* 实现状态机最简单的方法
```
void Heroine::handleInput(Input input) {
    switch (state_) {
    case STATE_STANDING:
        if (input == PRESS_B) {
            state_ = STATE_JUMPING;
            yVelocity_ = JUMP_VELOCITY;
            setGraphics(IMAGE_JUMP);
        }
        else if (input == PRESS_DOWN) {
            state_ = STATE_DUCKING;
            setGraphics(IMAGE_DUCK);
        }
        break;
    
    // other states...
    case STATE_JUMPING:
        if (input == PRESS_DOWN ) {
            state_ = STATE_DIVING;
            setGraphics(IMAGE_DIVE);
        }
        break;

    case STATE_DUCKING:
        if (input == RELEASE_DOWN) {
            state_ = STATE_STANDING;
            setGraphics(IMAGE_STAND);
        }
        break;
    }
}
```
* 状态模式
```
class HeroineState {
public:
    virtual ~HeroineState() {}
    virtual void handleInput(Heroine& heroine, Input input) {}
    virtual void update(Heroine& heroine) {}
};
```
* 为每一个状态定义一个类
```
class DuckingState : public HeroineState {
public:
    DuckingState() : chargeTime_(0) {}

    virtual void handleInput(Heroine& heroine, Input input) {
        if(input == RELEASE_DOWN) {
            // Change to standing state...
            heroine.setGraphics(IMAGE_STAND);
        }
    }

    virtual void handleInput(Heroine& heroine) {
        chargeTime_++;
        if(chargeTime_ > MAX_CHARGE) {
            heroine.superBomb();
        }
    }

private:
    int chargeTime_;
};
```
* 状态委托
```
class Heroine {
public:
    virtual void handleInput(Input input) {
        state_->handleInput(*this, input);
    }

    virtual void update() { state_->update(*this); }

private:
    HeroineState* state_;
}
```
* 将状态设置为静态变量的方法
```
class HeroineState {
public:
    static StandingState standing;
    static DuckingState ducking;
    static JumpingState jumping;
    static DivingState diving;

    // 其他代码
};
```
    - 在每个状态的 handleInput 方法有
```
    if(input == PRESS_B) {
        heroine.state_ = &HeroineState::jumping;
        heroine.setGraphics(IMAGE_JUMP);
    }
```
* 可以动态创建和删除
```
void Heroine::handleInput(Input input) {
    HeroineState* state = state_->handleInput(*this, input);
    if(state != NULL)
    {
        delete state_;
        state_ = state;
    }
}

HeroineState* StandingState::handleInput(Heroine& heroine, Input input) {
    if(input == PRESS_DOWN) {
        // 其他代码...
        return new DuckingState();
    }

    // 保持在当前状态.
    return NULL;
}
```
* 给状态添加进入状态和退出状态的行为
```
class StandingState : public HeroineState {
public:
    virtual void enter(Heroine& heroine) {
        heroine.setGraphics(IMAGE_STAND);
    }
}

void Heroine::handleInput(Input input) {
    HeroineState* state = state_->handleInput(*this, input);
    if(state != NULL)
    {
        delete state_;
        state_ = state;

        state_->enter(*this);
    }
}
```
* 并行状态机
```
class Heroine {
    // 其他代码

private:
    HeroineState* state_;
    HeroineState* equipment_;  // 武器状态
};
```
* 处理事件两种状态都进行处理
```
void Heroine::handleInput(Input input) {
    state_->handleInput(*this, input);
    equipment_->handleInput(*this, input);
}
```
* 层次状态机， 定义一个 "on ground" 的状态， 用来处理跳跃状态和躲避状态。 站立、走路、跑步、滑行从这个"on ground"状态继承而来, 并且在其类里面实现一些特殊行为。当一个事件进来的时候，如果子状态不处理它，那么它沿着继承链传给它的父状态来处理。
```
class OnGroundState : public HeroineState {
public:
    virtual void handleInput(Heroine& heroine, Input input) {
        if(input == PRESS_B) // Jump...
        else if(input == PRESS_DOWN) // Duck...
    }
};

class DuckingState : public OnGroundState {
public:
    virtual void handleInput(Heroine& heroine, Input input) {
        if (input == RELEASE_DOWN ) {
            // Stand up...
        }
        else {
            // 不处理输入， 由层次处理.
            OnGroundState::handleInput(heroine, input);
        }
    }
};
```
* 下推状态机， 新的状态进来后，压在栈顶，新的状态结束后可以恢复到原来的状态， 如开火后恢复到原来的状态。