#include "levelData.hpp"

#include <iostream>
#include <raylib.h>

LevelData::LevelData(int width, int height, std::string name, int tileSize)
 : width(width),
 height(height),
 name(name),
 tileSize(tileSize)
{
    level = std::vector<int>(width*height, 0);
    createEdges();
}

LevelData& LevelData::operator=(const LevelData &other)
{
    if (this == &other) {
        return *this;
    }
    this->level = other.level;
    this->edges = other.edges;

    this->width = other.width;
    this->height = other.height;
    this->name = other.name;
    this->tileSize = other.tileSize;

    return *this;
}

void LevelData::draw()
{
    for(int i=0; i<height; i++) {
        for(int j=0; j<width; j++) {
            int tileIndex = i*width + j;
            int xPos = j*tileSize;
            int yPos = i*tileSize;
            Color tileColor = WHITE;

            // std::vector<int>& tileMap = tileMaps[currentTileMapIndex];
            if (level.at(tileIndex) == 1) tileColor = BLUE;
            if (level[tileIndex] == 3) tileColor= ORANGE;


            DrawRectangle(xPos, yPos, tileSize, tileSize, tileColor);
        }
    }
}

// void LevelData::setWidth(int width)
// {
//     this->width = width;
// }

// void LevelData::setHeight(int height)
// {
//     this->height = height;
// }

void LevelData::setTileSize(int tileSize)
{
    this->tileSize = tileSize;
}

void LevelData::setName(std::string name)
{
    this->name = name;
}

int LevelData::get(int x, int y) const
{
    int index = y*width + x;
    if (index >= level.size() || index < 0 ) {
        std::cerr<<"ERROR [LevelData::get(int, int)] ("<<x<<","<<y<<"), id : "<<index<<" >= "<<level.size()<<"\n";
        return -1;
    }
    return level[index]; 
}

int LevelData::get(int index) const
{
    if (index >= level.size() || index < 0 ) {
        std::cerr<<"ERROR [LevelData::get(int)] id : "<<index<<" >= "<<level.size()<<"\n";
        return -1;
    }
    return level[index];
}

void LevelData::set(int x, int y, int value)
{
    int index = y*width + x;
    if (index >= level.size() || index < 0 ) {
        std::cerr<<"ERROR [LevelData::set(int, int)] ("<<x<<","<<y<<"), id : "<<index<<" >= "<<level.size()<<"\n";
        return;
    }
    level[index] = value;
}

void LevelData::set(int index, int value)
{
    if (index >= level.size() || index < 0 ) {
        std::cerr<<"ERROR [LevelData::set(int)] id : "<<index<<" >= "<<level.size()<<"\n";
        return;
    }
    level[index] = value;
}
Edge LevelData::getEdge(int index) const
{
    if (index >= edges.size() || index < 0 ) {
        std::cerr<<"ERROR [LevelData::getEdge] id : "<<index<<" >= "<<level.size()<<"\n";
        throw std::out_of_range("bad edge index");
    }
    return edges[index];
}

Edge& LevelData::getEdge(int index)
{
    if (index >= edges.size() || index < 0 ) {
        std::cerr<<"ERROR [LevelData::getEdge] id : "<<index<<" >= "<<level.size()<<"\n";
        throw std::out_of_range("bad edge index");
    }
    return edges[index];
}

void LevelData::setEdges(std::vector<Edge> edges)
{
    createEdges();
    if (this->edges.size() != edges.size()) {
        std::cerr<<"ERROR [LevelData::setEdges] created edges not equal assigend edges : "<<this->edges.size()<<" != "<<edges.size()<<"\n";
        return;
    }

    for(int i=0; i<this->edges.size(); i++) {
        this->edges[i].assignedNextLevel = edges[i].assignedNextLevel;
    }
}

void LevelData::createEdges()
{
    edges.clear();

    int widthCount = width;
    int heightCount = height;

    for(int i=0; i<widthCount; i++) {
        //top
        int index = i;
        if (level.at(index) == 0) {
            Edge edge;
            edge.type = TOP;
            edge.assignedNextLevel = -1;
            edge.tilesCount = 1;
            edge.start = {i, 0};
            edge.end = {(i+1), 0};

            if (i != 0 && !edges.empty() && edges.back().end.y == 0 && edges.back().end.x == edge.start.x) { 
                edges.back().end.x = (i+1);
                edges.back().tilesCount++;
            }
            else
                edges.push_back(edge);
        }
    }

    for(int i=0; i<widthCount; i++) {
        //bottom
        int index = (heightCount-1)*widthCount + i;
        if (level.at(index) == 0) {
            Edge edge;
            edge.type = DOWN;
            edge.assignedNextLevel = -1;
            edge.tilesCount = 1;
            edge.start = {i, (heightCount)};
            edge.end = {(i+1), (heightCount)};

            if (i != 0 && !edges.empty() && edges.back().end.y == (heightCount) && edges.back().end.x == edge.start.x) {
                edges.back().end.x = (i+1);
                edges.back().tilesCount++;
            }
            else 
                edges.push_back(edge);
        }
    }

    for(int j=0; j<heightCount; j++) {
        // left
        int index = j*widthCount;
        if (level.at(index) == 0) {
            Edge edge;
            edge.type = LEFT;
            edge.assignedNextLevel = -1;
            edge.tilesCount = 1;
            edge.start = {0, j};
            edge.end = {0, (j+1)};

            if (j != 0 && !edges.empty() && edges.back().end.x == 0 && edges.back().end.y == edge.start.y) {
                edges.back().end.y = (j+1);
                edges.back().tilesCount++;
            }
            else 
                edges.push_back(edge);
        }
    }


    for(int j=0; j<heightCount; j++) {
        // right
        int index = j*widthCount + (widthCount-1);
        if (level.at(index) == 0) {
            Edge edge;
            edge.type = RIGHT;
            edge.assignedNextLevel = -1;
            edge.tilesCount = 1;
            edge.start = {widthCount, j};
            edge.end = {widthCount, (j+1)};

            if (j != 0 && !edges.empty() && edges.back().end.x == widthCount && edges.back().end.y == edge.start.y) {
                edges.back().end.y = (j+1);
                edges.back().tilesCount++;
            }
            else 
                edges.push_back(edge);
        }
    }
}

void LevelData::changeLevelDimensions(int width, int height)
{
    std::vector<int> newLevel = std::vector<int>(width*height, 0);

    int minWidth = width > this->width ? this->width : width;
    int minHeight = height > this->height ? this->height : height;

    for(int i=0; i<minHeight; i++) {
        for(int j=0; j<minWidth; j++) {
            int index = i*width + j;
            if (index >= newLevel.size()) {
                std::cerr<<"[LevelData::changeLevelDimensions] wrong index: "<<index<<", should be "<<newLevel.size()<<"\n";
                return;
            }
            int index2 = i*this->width + j;
            if (index2 >= level.size()) {
                std::cerr<<"[LevelData::changeLevelDimensions] wrong index: "<<index2<<", should be "<<level.size()<<"\n";
                return;
            }
            newLevel[index] = level[index2];
        }
    }

    level = newLevel;
    this->width = width;
    this->height = height;
}