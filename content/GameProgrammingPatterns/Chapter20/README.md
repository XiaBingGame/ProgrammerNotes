# 第20章 空间分区
* 将对象存储在根据位置组织的数据结构中来高效地定位它们.
* 虚拟现实中将要关注的一点就是位置, 游戏世界具有空间感. 如物理, 如音频, 更远的声音会更安静.
* 游戏引擎需要常常查询"对象的附近有什么物体"
* 如遍历战场任何对象附近的所有对象进行攻击
```
void handleMelee(Unit* units[], int numUnits) {
    for(int a = 0; a < numUnits - 1; a++) {
        for(int b = a + 1; b < numUnits; b++) {
            if(units[a]->position() == units[b]->position()) {
                handleAttack(units[a], units[b]);
            }
        }
    }
}
```
* 空间分区模式: 对于一组对象而言, 每一个对象在空间都有一个位置, 将对象存储在一个根据对象的位置来组织的数据结构中, 该数据结构可以让你高效地查询位于或靠近某处的对象. 当对象的位置变化时, 应更新该空间数据结构以便可以继续这样查找对象.

## 使用情境
* 有一组对象, 每个对象具备某种位置信息, 且需要根据位置做大量的查询来查找对象从而遇到了性能问题.

## 使用须知
* 该模式较复杂, 所以对象数量越多越划算
* 该模式以空间换取速度.

## 示例代码
```
class Unit {
    friend class Grid;

public:
    Unit(Grid* grid, double x, double y);

    void move(double x, double y);

private:
    double x_, y_;
    Grid* grid_;

    Unit* prev_;
    Unit* next_;
};

Unit::Unit(Grid* grid, double x, double y)
    : grid_(grid)
    , x_(x)
    , y_(y) 
    , prev_(NULL)
    , next_(NULL) {
        grid_->add(this);
    }

void Unit::move(double x, double y) {
    grid_->move(this, x, y);
}

class Grid {
public:
    Grid() {
        for (int x = 0; x < NUM_CELLS; x++) {
            for (int y = 0; y < NUM_CELLS; y++) {
                celss_[x][y] = NULL;
            }
        }
    }

    void add(Unit* unit);
    void handleMelee();
    void handleCell(Unit* unit);
    void move(Unit* unit, double x, double y);

    static const int NUM_CELLS = 10;
    static const int CELL_SIZE = 20;

private:
    Unit* cells_[NUM_CELLS][NUM_CELLS];
};

void Grid::add(Unit* unit) {
    int cellX = (int)(unit_->x_ / Grid::CELL_SIZE);
    int celly = (int)(unit_->y_ / Grid::CELL_SIZE);

    unit->prev_ = NULL;
    unit->next_ = cells_[cellX][cellY];
    cells_[cellX][cellY] = unit;

    if(unit->next_ != NULL) {
        unit->next_->prev_ = unit;
    }
}

void Grid::handleMelee() {
    for(int x = 0; x < NUM_CELLS; x++) {
        for(int y = 0; y < NUM_CELLS; y++) {
            handleCell(cells[x][y]);
        }
    }
}

void Grid::handleCell(Unit* unit) {
    while(unit != NULL) {
        Unit* other = unit->next_;
        while(other != NULL) {
            if(unit->x_ == other->x_ &&
                unit->y_ == other->y_) {
                    handleAttack(unit, other);
                }
            other = other->next_;
        }

        unit = unit->next_;
    }
}

void Grid::move(Unit* unit, double x, double y) {
    int oldCellX = (int)(unit->x_ / Grid::CELL_SIZE);
    int oldCellY = (int)(unit->y_ / Grid::CELL_SIZE);

    int cellX = (int)(unit->x_ / Grid::CELL_SIZE);
    int cellY = (int)(unit->y_ / Grid::CELL_SIZE);

    unit->x_ = x;
    unit->y_ = y;

    if(oldCellX == cellX && oldCellY == cellY) return;

    if(unit->prev_ != NULL) {
        unit->prev_->next_ = unit->next_;
    }

    if(unit->next_ != NULL) {
        unit->next_->prev_ = unit->prev_;
    }

    // 解决单元网格的首元素问题
    if(cells_[oldCellX][oldCellY] == unit) {
        cells_[oldCellX][oldCellY] = unit->next;
    }

    add(unit);
}
```
* 当不需要精确匹配时, 可以使用另外的判断语句
```
if(distance(unit, other) < ATTACK_DISTANCE) {
    handleAttack(unit, other);
}
```
* 比较相邻单元格的单位
```
// 比较两个对象是否足够近
void Grid::handleUnit(Unit* unit, Unit* other) {
    while (other != NULL) {
        if ( distance(unit, other) < ATTACK_DISTANCE ) {
            handleAttack(unit, other);
        }

        other = other->next_;
    }
}

// 比较一个网格内的对象
void Grid::handleCell(int x, int y) {
    Unit* unit = cells_[x][y];
    while(unit != NULL) {
        // 处理单元格内的其他对象
        handleUnit(unit, unit->next_);

        // 处理网格内的其他对象
        // 只查看一半相邻的单元格, 避免比较两次
        if(x > 0) handleUnit(unit, cells_[x - 1][y]);
        if(y > 0) handleUnit(unit, cells_[x][y - 1]);
        if(x > 0 && y > 0)
            handleUnit(unit, cells_[x - 1][y - 1]);
        if(x > 0 && y < NUM_CELLS - 1)
            handleUnit(unit, cells_[x - 1][y + 1]);

        unit = unit->next_;
    }
}
```
* 分区除了是上面扁平的, 还可以是层级的, 层级空间分区则是空间划分成几个区域. 如果这些区域中仍然包含着许多的对象,就会继续划分,这个递归过程持续到每个区域的对象数目都少于某个约定的最大对象数量为止.
* 二叉空间分割(BPSs) 和 k-d 树(k-d tree)空间分区方式.
* 四叉树
* 常见的结构
    - 网格
    - 四叉树
    - 二叉空间分割
    - k-dimensional 树
    - 层次包围盒
* 空间数据结构对应的线性结构
    - 网格是一个连续的桶排序
    - 二叉空间分割, k-d树, 以及层级包围盒都是二叉查找树
    - 四叉树和八叉树都是 Trie 树