* 数据可以分为两类，一个通用的，一个各自有状态的
```
// 通用数据
class TreeModel {
private:
    Mesh mesh_;
    Texture bark_;
    Texture leaves_;
};

// 包含外部状态的数据
class Tree {
private:
    TreeModel* model_;

    Vector position_;
    double height_
    double thickness_
    Color barkTint_;
    Color leafTint_;
};
```
* 地形实现
···
enum Terrain {
    TERRAIN_GRASS,
    TERRAIN_HILL,
    TERRAIN_RIVER,
    // ...
};

class World {
private:
    Terrain tiles_[WIDTH][HEIGHT];
}

int World::getMovementCost(int x, int y) {
    switch (tiles_[x][y]) {
    case TERRAIN_GRASS: return 1;
    case TERRAIN_HILL: return 3;
    case TERRAIN_RIVER: return 2;
    // ...
    }
}

bool World::isWater(int x, int y) {
    switch (tiles_[x][y]) {
    case TERRAIN_GRASS: return false;
    case TERRAIN_HILL: return false;
    case TERRAIN_RIVER: return true;
    // ...
    }
}
···
* 面向对象实现
```
class Terrain {
public:
    Terrain(int moveCost, bool isWater, Texture texture)
        : moveCost_(moveCost)
        , isWater_(isWater)
        , texture_(texture)
    {}

    int getMoveCost() const { return moveCost_;  }
    bool isWater() const { return isWater_;  }
    const Texture& getTexture() const { return texture_;  }

private:
    int moveCost_;
    bool isWater_;
    Texture texture_;
}

class World {
public:
    World() 
    : grassTerrain_(1, false, GRASS_TEXTURE)
    : hillTerrain_(1, false, HILL_TEXTURE)
    : riverTerrain_(1, false, RIVER_TEXTURE)
    {}
    void generateTerrain();
    const Terrain& getTile(int x, int y) const;

private:
    Terrain* tiles_[WIDTH][HEIGHT];
    Terrain grassTerrain_;
    Terrain hillTerrain_;
    Terrain riverTerrain_;
};

void World::generateTerrain(){
    for(int x = 0; x < WIDTH; x++) {
        for(int y = 0; y < HEIGHT, y++) {
            if(random(10) == 0) {
                tiles_[x][y] = &hillTerrain_;
            }
            else {
                tiles_[x][y] = &grassTerrain_;
            }
        }
    }
    int x = random(WIDTH);
    for(int y = 0; y < HEIGHT, y++) {
        tiles_[x][y] = &riverTerrain_;
    }
}

const Terrain& World::getTile(int x, int y) const {
    return *tiles_[x][y];
}
```