#pragma once

#include <vector>
#include <string>
#include "app/common.hpp"
#include "core/grid.hpp"
#include <map>

class LevelData {
private:
    Grid<int> level;
    Grid<CellType> collision;
    std::vector<Edge> edges;
    std::map<ExitKey, LevelId> links;

    int tileSize;
    
    std::string name;

    void createEdges();
    void rebuildEdges();

public:
    LevelData(int width, int height, std::string name = "level", int tileSize = 64);
    LevelData& operator=(const LevelData& other);

    int getWidth() const {return level.getWidth();};

    int getHeight() const {return level.getHeight();};

    int getTileSize() const {return tileSize;};
    void setTileSize(int tileSize);

    std::string getName() const {return name;};
    void setName(std::string name);

    int getLevelSize() const {return static_cast<int>(level.size());};
    int getEdgesSize() const {return static_cast<int>(edges.size());};

    int get(int x, int y) const;
    int get(int index) const;

    CellType getTileCollision(int spriteId) const {return collision.at(spriteId);};

    void set(int x, int y, int value);
    void set(int index, int value);


    Edge& getEdge(int index);
    Edge getEdge(int index) const;
    void setEdges(std::vector<Edge> edges);

    void changeLevelDimensions(int width, int height);

    void setLink(const ExitKey& key, LevelId targetId);
    LevelId getLink(const ExitKey& key) const;
    const std::map<ExitKey, LevelId>& getLinks() const {return links; }


};
