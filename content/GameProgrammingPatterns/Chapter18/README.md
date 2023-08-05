# 第18章 脏标记模式
* 游戏通常有称之为场景图的东西, 表示游戏世界中所有的物体.
* 缓存每一个物体的世界变换, 以避免每一帧都计算该物体的世界变换. 这样渲染的时候, 只使用预先计算好的世界变换, 如果物体从不移动, 则缓存的变换始终是最新的.
* 给某个变换一个标记, 用于标记它是否需要更新. 当渲染的时候需要这个物体的变换时, 如果标记为真, 则计算该变换, 然后清空该标记.
* 确保每次原始数据的改动都要设置脏标记. 一个解决方法就是将原始数据的改动封装起来, 任何可能的变动都通过单一 API 入口来修改, 可以在这里设置脏标记, 并且不用担心会有遗漏.
```
class Transform {
public:
    static Transform origin();
    Transform combine(Transform& other);  // 和其他变换组合起来,比较费时的操作
};

class GraphNode {
public:
    GraphNode(Mesh* mesh)
        : mesh_(mesh)
        , local_(Transform::origin()) 
        , dirty_(true)
        { }

    // 修改本地位置
    void setTransform(Transform local);
    void render(Transform parentWorld, bool dirty);

private:
    Transform local_;
    Transform world_;
    bool dirty_;

    Mesh* mesh_;

    GraphNode* children_[MAX_CHILDREN];
    int numChildren_;
};

void setTransform(Transform local) {
    local_ = local;
    dirty_ = true;
}

void GraphNode::render(Transform parentWorld, bool dirty) {
    dirty |= dirty_;
    
    if(dirty) {
        world_ = local_combine(parentWorld);
        dirty_ = false;
    }

    if(mesh_) renderMesh(mesh_, world);

    for(int i = 0; i < numChildren_; i++) {
        children_[i]->render(world, dirty);
    }
}

// 从根节点开始渲染
graph_->render(Transform::origin());
```
* 渲染时传递父节点的脏标记到它的子节点中, 并检查传递的标记来确认是否有需要重新计算世界变换.