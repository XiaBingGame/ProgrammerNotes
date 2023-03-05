* 用一个基类代表游戏命令, 然后实现各种命令
```
class Command {
public:
    virtual ~Command() {}
    virtual void execute() = 0;
};

class JumpCommand : public Command {
public:
    virtual void execute() { jump(); }
};

class FireCommand : public Command {
public:
    virtual void execute() { fireGun(); }
};

class InputHandler {
public:
    void handleInput();

private:
    Command* buttonX_;
    Command* buttonY_;
    Command* buttonA_;
    Command* buttonB_;
};

void InputHandler::handleInput()
{
    if(isPressed(BUTTON_X)) buttonX_->execute();
    if(isPressed(BUTTON_Y)) buttonY_->execute();
    if(isPressed(BUTTON_A)) buttonA_->execute();
    if(isPressed(BUTTON_B)) buttonB_->execute();
}
```
* 添加角色
```
class Command {
public:
    virtual ~Command() {}
    virtual void execute(GameActor& actor) = 0;
};

class JumpCommand : public Command {
public:
    virtual void execute(GameActor& actor) { actor.jump(); }
};

class FireCommand : public Command {
public:
    virtual void execute(GameActor& actor) { actor.fireGun(); }
};

class InputHandler {
public:
    Command* handleInput();

private:
    Command* buttonX_;
    Command* buttonY_;
    Command* buttonA_;
    Command* buttonB_;
};

Command* InputHandler::handleInput() {
    if(isPressed(BUTTON_X)) return buttonX_;
    if(isPressed(BUTTON_Y)) return buttonY_;
    if(isPressed(BUTTON_A)) return buttonA_;
    if(isPressed(BUTTON_B)) return buttonB_;

    return NULL;
}

Command* command = inputHandler.handleInput();
if(command) {
    command->execute(actor);
}
```
* 一次性命令实现可撤销命令
···
class Command {
public:
    virtual ~Command() {}
    virtual void execute() = 0;
    virtual void undo() = 0;
}

class MoveUnitCommand : public Command {
public:
    MoveUnitCommand(Unit* unit, int x, int y)
        : unit_(unit), x_(x), y_(y), xBefore_(0), yBefore(0) {}
    virtual void execute() {
        xBefore_ = unit_->x();
        yBefore_ = unit_->y();
        unit_->moveTo(x_, y_);
    }
    virtual void undo() {
        unit_->moveTo(xBefore_, yBefore_);
    }

private:
    Unit* unit_;
    int x_, y_;
    int xBefore_, yBeforee_;
};

Command* handleInput() {
    Unit* unit = getSeletedUnit();

    if(isPressed(BUTTON_UP)) {
        int destY = unit->y() - 1;
        return new MoveUnitCommand(unit, unit->x(), destY);
    }
    if(isPressed(BUTTON_DOWN)) {
        int destY = unit->y() + 1;
        return new MoveUnitCommand(unit, unit->x(), destY);
    }
    // ...
    
    return NULL;
}
···