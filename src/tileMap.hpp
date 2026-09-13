#pragma once

#include <vector>
#include "tileLevelManager.hpp"
#include "raylib.h"
#include "common.hpp"

class TileMap
{
private:
    
    int m_tileSize = 64;
    TileLevelManager& tileLevelManager;
    int currentTileMapIndex = 0;

    
    public:
    TileMap(TileLevelManager& tileLevelManager); 
    ~TileMap() = default;
    
    void update();
    void draw();
    
    int getTileSize() {return m_tileSize;}
    bool isCollision(Vector2 position, int tileType) const;
    Vector2 getSpawnPosition();
    Vector2i getSize();
    
    std::vector<Edge> getEdges();
    
    bool checkForNextLevel(Vector2& playerPosition);
    void changeLevel(int levelID);
};

