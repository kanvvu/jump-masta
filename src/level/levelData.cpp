#include "levelData.hpp"

#include <iostream>
#include <raylib.h>
#include "assets/assets.hpp"

LevelData::LevelData(int width, int height, std::string name, int tileSize)
 : name(name),
   tileSize(tileSize)
{
    level = Grid<int>(width, height, 0);
    collision = Grid<CellType>(width, height, CellType::Empty);
    createEdges();
}

LevelData& LevelData::operator=(const LevelData &other)
{
    if (this == &other) {
        return *this;
    }
    this->level = other.level;
    this->collision = other.collision;
    this->edges = other.edges;
    this->links = other.links;

    this->name = other.name;
    this->tileSize = other.tileSize;

    return *this;
}


void LevelData::setTileSize(int _tileSize)
{
    this->tileSize = _tileSize;
}

void LevelData::setName(std::string _name)
{
    this->name = _name;
}

int LevelData::get(int x, int y) const
{
    int index = y*getWidth() + x;
    if (index >= level.size() || index < 0 ) {
        std::cerr<<"ERROR [LevelData::get(int, int)] ("<<x<<","<<y<<"), id : "<<index<<" >= "<<level.size()<<"\n";
        return -1;
    }
    return level.at(index);
}

int LevelData::get(int index) const
{
    if (index >= level.size() || index < 0 ) {
        std::cerr<<"ERROR [LevelData::get(int)] id : "<<index<<" >= "<<level.size()<<"\n";
        return -1;
    }
    return level.at(index);
}

void LevelData::set(int x, int y, int value, CellType cellType)
{
    int index = y*getWidth() + x;
    if (index >= level.size() || index < 0 ) {
        std::cerr<<"ERROR [LevelData::set(int, int)] ("<<x<<","<<y<<"), id : "<<index<<" >= "<<level.size()<<"\n";
        return;
    }
    level.at(index) = value;
    collision.at(index) = cellType;

    if(x == 0 || y == 0 || x == getWidth() - 1 || y == getHeight() - 1)
        createEdges();
}

void LevelData::set(int index, int value, CellType cellType)
{
    if (index >= level.size() || index < 0 ) {
        std::cerr<<"ERROR [LevelData::set(int)] id : "<<index<<" >= "<<level.size()<<"\n";
        return;
    }
    level.at(index) = value;
    collision.at(index) = cellType;

    int x = index % getWidth();
    int y = index/getWidth();

    if (y*getWidth()+ x != index) {
        std::cerr<<"ERROR [LevelData::set(int)]  ("<<x<<", "<<y<<") "<<index<<" != "<<y*getWidth()+ x<<"\n";
        return;
    }

    if(x == 0 || y == 0 || x == getWidth() - 1 || y == getHeight() - 1)
        createEdges();
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

void LevelData::setEdges(std::vector<Edge> _edges)
{
    createEdges();
    if (this->edges.size() != _edges.size()) {
        std::cerr<<"ERROR [LevelData::setEdges] created edges not equal assigend edges : "<<this->edges.size()<<" != "<<_edges.size()<<"\n";
        return;
    }

    for(int i=0; i<this->edges.size(); i++) {
        this->edges[i].assignedNextLevel = _edges[i].assignedNextLevel;
        if (edges[i].assignedNextLevel != -1) {
            setLink(edges[i].getKey(), edges[i].assignedNextLevel);
        }
    }
}

void LevelData::createEdges()
{
    // edges.clear();
    std::vector<Edge> newEdges;

    int widthCount = getWidth();
    int heightCount = getHeight();

    for(int i=0; i<widthCount; i++) {
        //top
        int index = i;
        if (collision.at(index) == CellType::Empty) {
            Edge edge;
            edge.type = EdgeType::TOP;
            edge.assignedNextLevel = -1;
            edge.tilesCount = 1;
            edge.start = {i, 0};
            edge.end = {(i+1), 0};

            if (i != 0 && !newEdges.empty() && newEdges.back().end.y == 0 && newEdges.back().end.x == edge.start.x) { 
                newEdges.back().end.x = (i+1);
                newEdges.back().tilesCount++;
            }
            else
                newEdges.push_back(edge);
        }
    }

    for(int i=0; i<widthCount; i++) {
        //bottom
        int index = (heightCount-1)*widthCount + i;
        if (collision.at(index) == CellType::Empty) {
            Edge edge;
            edge.type = EdgeType::DOWN;
            edge.assignedNextLevel = -1;
            edge.tilesCount = 1;
            edge.start = {i, (heightCount)};
            edge.end = {(i+1), (heightCount)};

            if (i != 0 && !newEdges.empty() && newEdges.back().end.y == (heightCount) && newEdges.back().end.x == edge.start.x) {
                newEdges.back().end.x = (i+1);
                newEdges.back().tilesCount++;
            }
            else 
                newEdges.push_back(edge);
        }
    }

    for(int j=0; j<heightCount; j++) {
        // left
        int index = j*widthCount;
        if (collision.at(index) == CellType::Empty) {
            Edge edge;
            edge.type = EdgeType::LEFT;
            edge.assignedNextLevel = -1;
            edge.tilesCount = 1;
            edge.start = {0, j};
            edge.end = {0, (j+1)};

            if (j != 0 && !newEdges.empty() && newEdges.back().end.x == 0 && newEdges.back().end.y == edge.start.y) {
                newEdges.back().end.y = (j+1);
                newEdges.back().tilesCount++;
            }
            else 
                newEdges.push_back(edge);
        }
    }


    for(int j=0; j<heightCount; j++) {
        // right
        int index = j*widthCount + (widthCount-1);
        if (collision.at(index) == CellType::Empty) {
            Edge edge;
            edge.type = EdgeType::RIGHT;
            edge.assignedNextLevel = -1;
            edge.tilesCount = 1;
            edge.start = {widthCount, j};
            edge.end = {widthCount, (j+1)};

            if (j != 0 && !newEdges.empty() && newEdges.back().end.x == widthCount && newEdges.back().end.y == edge.start.y) {
                newEdges.back().end.y = (j+1);
                newEdges.back().tilesCount++;
            }
            else 
                newEdges.push_back(edge);
        }
    }

    for(auto& edge : newEdges) {
        int levelId = getLink(edge.getKey());
        edge.assignedNextLevel = levelId;
    }
    
    edges = newEdges;
}

void LevelData::changeLevelDimensions(int newWidth, int newHeight)
{
    level.resize(newWidth, newHeight, 0);
    collision.resize(newWidth, newHeight, CellType::Empty);

    createEdges();
}

void LevelData::rebuildCollision(Assets& assets)
{
    for(int i=0; i<level.size(); i++) {
        collision.at(i) = assets.getCellType(level.at(i));
    }
    createEdges();
}

void LevelData::setLink(const ExitKey &key, LevelId targetId)
{
    if (targetId == -1) {
        links.erase(key);
    } else {
        links[key] = targetId;
    }

    for(auto& edge : edges) {
        if (edge.getKey() == key) {
            edge.assignedNextLevel = targetId;
        }
    }
}

LevelId LevelData::getLink(const ExitKey &key) const
{
    auto it = links.find(key);
    return (it != links.end()) ? it->second : -1;
}
