#pragma once

#include <vector>
#include <string>
#include "app/common.hpp"
#include "assets/sprite.hpp"

class Assets;

struct Tile {
    int type;
    Sprite sprite;
};

class LevelData {
private:
    std::vector<int> level;
    std::vector<CellType> collision;
    std::vector<Edge> edges;

    int width;
    int height;
    int tileSize;
    
    std::string name;

    void createEdges();
    void rebuildEdges();

public:
    LevelData(int width, int height, std::string name = "level", int tileSize = 64);
    LevelData& operator=(const LevelData& other);

    int getWidth() const {return width;};
    // void setWidth(int width);

    int getHeight() const {return height;};
    // void setHeight(int height);

    int getTileSize() const {return tileSize;};
    void setTileSize(int tileSize);

    std::string getName() const {return name;};
    void setName(std::string name);

    int getLevelSize() const {return static_cast<int>(level.size());};
    int getEdgesSize() const {return static_cast<int>(edges.size());};

    int get(int x, int y) const;
    int get(int index) const;

    CellType getTileCollision(int spriteId) const {return collision[spriteId];};

    void set(int x, int y, int value, CellType cellType = CellType::Empty);
    void set(int index, int value, CellType cellType = CellType::Empty);


    Edge& getEdge(int index);
    Edge getEdge(int index) const;
    void setEdges(std::vector<Edge> edges);

    void changeLevelDimensions(int width, int height);

    void rebuildCollision(Assets& assets);


};
