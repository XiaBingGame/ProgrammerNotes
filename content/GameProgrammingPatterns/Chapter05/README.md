* 原型模式, 原型基类有接口可以创建自身类型的对象, 生成器是用原型基类类型生成不同种类的原型.
* 怪物基类, 有接口生成自身
```
class Monster {
public:
    virtual ~Monster() {}
    virtual Monster* clone() = 0;

    // 其他函数
};
```
* 具体怪物
```
class Ghost : public Monster {
public:
    Ghost(int health, int speed)
        : health_(health)
        , speed_(speed)
    {}

    virtual Monster* clone() {
        return new Ghost(health_, speed_);
    }

private:
    int health_;
    int speed_;
};
```
* 生成器
```
class Spawner {
public:
    Spawner(Monster* prototype)
        : protype_(prototype)
    {}

    Monster* spawnMonster() {
        return prototype_->clone();
    }

private:
    Monster* prototype_;
};
```
* 生成
```
Monster* ghostPrototype = new Ghost(15, 3);
Spawner* ghostSpawner = new Spawner(ghostPrototype);

Monster* = ghostSpawner->spawnMonster();
```
* 使用生成器函数代替生成器类
```
Monster* spawnGhost() {
    return new Ghost();
}

typedef Monster* (*SpawnCallback)();

class Spawner {
public:
    Spawner(SpawnCallback spawn)
        : spawn_(spawn)
    {}

    Monster* spawnMonster() { return spawn_(); }

private:
    SpawnCallback spawn_;
};

// 创建生成器
Spawner* ghostSpawner = new Spawner(spawnGhost);
```
* 使用模板创建生成器
```
class Spawner {
public:
    virtual ~Spawner() {}
    virtual Monster* spawnMonster() = 0;
};

template <class T>
class SpawnerFor : public Spawner {
public:
    virtual Monster* spawnMonster() { return new T(); }
};

Spawner* ghostSpawner = new SpawnerFor<Ghost>();
```