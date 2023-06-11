* 通过合理组织数据利用 CPU 的缓存机制来加快内存访问速度
* 芯片内存很小,以便嵌入在芯片上,而且由于它使用了更快的内存类型(静态RAM或称"SRAM"),所以更加昂贵.这一小块内存被称之为缓存(芯片上的那块内存便是L1缓存)
* 优化的目标是将你的数据结构进行组织,以使需要处理的数据对象在内存中两两相邻.
* 使用数据局部性的第一条准则就是找到出现性能问题的地方.不要在那些代码库里非频繁执行的部分浪费时间.
* 可以用 Cachegrind 免费工具测量缓存命中的情况
* 为了做到缓存友好,你可能需要牺牲一些之前所做的抽象化.越是在程序的数据局部性上下功夫, 就越要牺牲继承、接口以及这些手段所带来的好处.
* 假设有一系列游戏实体, 每个实体通过组件模式被拆解为不同的域: AI、物理、渲染.
```
class GameEntity {
public:
    GameEntity(AIComponent* ai,
            PhysicsComponent* physics,
            RenderComponent* render)
    : ai_(ai), physics_(physics), render_(render)
    {}

    AIComponent* ai() { return ai_; }
    PhysicsComponent* physics() { return physics_; }
    RenderComponent* render() { return render_; }
    

private:
    AIComponent* ai_;
    PhysicsComponent* physics_;
    RenderComponent* render_;
};
```
* 假设每个组件包含一些状态, 如向量和矩阵.
```
class AIComponent {
public:
    void update() {
        // 工作并修改状态...
    }

private:
    // 目标, 情绪等
};

class PhysicsComponent {
public:
    void update() {
        // 工作并修改状态...
    }

private:
    // 刚体, 速度, 质量等
};

class RenderComponent {
public:
    void update() {
        // 工作并修改状态...
    }

private:
    // 网格, 纹理, 着色器等
};
```
* 每次游戏循环需要做以下工作:
    - 为所有实体更新 AI 组件
    - 为所有实体更新其物理组件
    - 使用渲染组件对它们进行渲染.
```
while(!gameOver) {
    for(int i = 0; i < numEntities; i++) {
        entities[i]->ai()->update();
    }
    for(int i = 0; i < numEntities; i++) {
        entities[i]->physics()->update();
    }
    for(int i = 0; i < numEntities; i++) {
        entities[i]->render()->update();
    }

    // 其他游戏机制
}
```
* 假如你希望增强数据局部性, 就尽可能想办法去掉那些间接性的(尤其是指针的)操作
* 取消指针跟踪, 对三个连续数组进行遍历来取代内存中跳跃性的访问
```
AIComponent* aiComponents = new AIComponent[MAX_ENTITIES];
PhysicsComponent* physicsComponents = new PhysicsComponent[MAX_ENTITIES];
RenderComponent* renderComponents = new RenderComponent[MAX_ENTITIES];

while(!gameOver) {
    for(int i = 0; i < numEntities; i++) {
        aiComponents[i]->update();
    }

    for(int i = 0; i < numEntities; i++) {
        physicsComponents[i]->update();
    }

    for(int i = 0; i < numEntities; i++) {
        renderComponents[i]->update();
    }
}
```
* 假设在设置一个粒子系统, 将所有的粒子置入一个大的连续数组中, 然后封装成一个管理类
```
class Particle {
public:
    void update() { /* 重力, 等 */ }
    // 位置, 速度, 等
};

class ParticleSystem {
public:
    ParticleSystem() : numParticles_(0) { }

    void update();
private:
    static const int MAX_PARTICLES = 100000;
    int numParticles_;
    Particle particles_[MAX_PARTICLES];
};

void ParticleSystem::update() {
    for(int i = 0; i < numParticles_; i++) {
        if(particles_[i].isActive())
            particles_[i].update();
    }
}
```
* 由于CPU的分支预测测性, 对于控制流语句, 一旦预测失败, 则会产生较大的性能损耗
* 如果粒子已排序, 可以仅更新前面活跃的粒子
```
for(int i = 0; i < numActive_; i++) {
    particles[i].update();
}
```
* 当粒子被激活时, 把它与数组中第一个未激活的粒子进行交换来将其移动到所有激活粒子的末端
```
void ParticleSystem::activeParticle(int index) {
    // 不要激活已激活的粒子
    assert(index >= numActive_);

    Particle temp = particles_[numActive_];
    particles_[numActive_] = particles_[index];
    partices_[index] = temp;

    numActive_++;
}

void ParticleSystem::deactiveParticle(int index) {
    // 不要取消激活未激活的粒子
    assert(index < numActive_);

    numActive_--;

    Particle temp = particles_[numActive_];
    particles_[numActive_] = particles_[index];
    partices_[index] = temp;
}
```
* 由于粒子无法确定自己在粒子系统数组的位置, 因此不再自己控制激活和反激活, 而是由粒子系统控制它的激活和反激活.
* 假设实体的 AI 组件包含了一些状态: 如每帧都要检查的状态有当前播放的动画, 所走向的目标位置, 能量值等, 不是每帧都要检查的变量, 如被开枪打死后掉落宝物的数据(仅在生命周期结束时才被使用).
```
class AIComponent {
public:
    void update() { /* ... */ }

private:
    // 每帧都检查的变量
    Animation* animation_;
    double energy_;
    Vector goalPos_;

    // 非每帧都检查的变量
    LootType drop_;
    int minDrops_;
    int maxDrops_;
    double chanceOfDrop_;
};
```
* 上面的数据分为两部分, 一部分为"热数据", 即每帧需要用到的数据, 另外一部分为"冷数据", 不会被频繁使用到的剩余数据.
```
class AIComponent {
public:
    void update() { /* ... */ }

private:
    // 每帧都检查的变量
    Animation* animation_;
    double energy_;
    Vector goalPos_;

    // 非每帧都检查的变量
    LoopDrop* loot;
};

class LootDrop {
    friend class AIComponent;
    LootType drop_;
    int minDrops_;
    int maxDrops_;
    double chanceOfDrop_;
};
```
* 如何处理多态
    - 避开继承
    - 为不同的对象类型使用相互独立的数组
    - 使用指针集合
* 如何定义游戏实体
    - 游戏实体通过类中的指针来索引其组件
    - 游戏实体通过一系列ID来索引组件
    - 游戏实体本身就是ID, 组件通过相同的ID找到实体.
* Tony Albrecht 写作的 Pitfalls of Object-Oriented Programming 
* Noel Lopis 的博客