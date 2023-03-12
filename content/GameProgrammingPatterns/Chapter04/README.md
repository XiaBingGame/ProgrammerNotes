* 实现"从桥上坠落"的成就.
```
void Physics::updateEntity(Entity& entity) {
    bool wasOnSurface = entity.isOnSurface();
    entity.accelerate(GRAVITY);
    entity.update();
    if ( wasOnSurface && !entity.isOnSurface() )
    {
        // 通过下面的代码进行通知.
        notify(entity, EVENT_START_FELL);
    }
}
```
* 观察者
```
class Observer {
public:
    virtual ~Observer() {}
    virtual void onNotify(const Entity& entity, Event event) = 0;
};

class Achievements : public Observer {
public:
    virtual void onNotify(const Entity& entity, Event event) {
        switch(event) {
        case EVENT_ENTITY_FELL:
            if(entity.isHero() && heroIsOnBridge_) {
                unlock(ARCHIEVEMENT_FELL_OFF_BRIDGE);
            }
            break;

        // 处理其他事件
        }
    }
private:
    void unlock(Achievement achievement) {
        // 解锁该成就
    }
};
```
* 被观察者
```
class Subject {
public:
    void addObserver(Observer* observer) {
        // 添加至数组
    }

    void removeObserver(Observer* observer) {
        // 从数组中移除
    }

protected:
    void notify(const Entity& entity, Event event) {
        for(int i = 0; i < numObservers_; i++) {
            observers_[i]->onNotify(entity, event);
        }
    }

private:
    Observer* observers_[MAX_OBSERVERS];
    int numObservers_;
};
```
* 使用链表实现观察者列表
```
class Subject {
public:
    Subject() : header_(NULL) {}
    void addObserver(Observer* observer) {
        observer->next_ = header_;
        header_ = observer;
    }

    void removeObserver(Observer* observer) {
        if(header_ == observer) {
            header_ = observer->next_;
            observer->next_ = NULL;
            return;
        }

        Observer* current = header_;
        while(current != NULL)
        {
            if(current->next_ == observer)
            {
                current->next_ = observer->next_;
                observer->next_ = NULL;
                return;
            }

            current = current->next_;
        }
    }

protected:
    void notify(const Entity& entity, Event event) {
        Observer* observer = header_;
        while(observer != NULL)
        {
            observer->notify(entity, event);

            observer = observer->next_;
        }
    }

private:
    Observer* header_;
};

class Observer {
    friend class Subject;

public:
    Observer() : next_(NULL) {}

private:
    // 其他观察者
    Observer* next_;
};
```