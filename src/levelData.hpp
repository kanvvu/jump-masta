#pragma once

#include <vector>
#include <string>
#include "common.hpp"

class LevelData {
private:
    std::vector<int> level;
    std::vector<Edge> edges;

    int width;
    int height;
    int tileSize;
    
    std::string name;

    void createEdges();
public:
    LevelData(int width, int height, std::string name = "level", int tileSize = 64);
    LevelData& operator=(const LevelData& other);


    void draw();

    int getWidth() const {return width;};
    // void setWidth(int width);

    int getHeight() const {return height;};
    // void setHeight(int height);

    int getTileSize() const {return tileSize;};
    void setTileSize(int tileSize);

    std::string getName() const {return name;};
    void setName(std::string name);

    int getLevelSize() const {return level.size();};
    int getEdgesSize() const {return edges.size();};

    int get(int x, int y) const;
    int get(int index) const;

    void set(int x, int y, int value);
    void set(int index, int value);

    Edge& getEdge(int index);
    Edge getEdge(int index) const;
    void setEdges(std::vector<Edge> edges);

    void changeLevelDimensions(int width, int height);


};