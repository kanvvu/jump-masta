#pragma once

#include <vector>
#include "tileLevelManager.hpp"
#include "raylib.h"
#include "common.hpp"

class TileMap
{
private:
    
    const int m_widthCount;
    const int m_heightCount;
    const int m_tileSize = 64;


    TileLevelManager& tileLevelManager;
    int currentTileMapIndex = 0;

public:
    TileMap(TileLevelManager& tileLevelManager, int widthCount, int heightCount);
    ~TileMap() = default;

    void update();
    void draw();

    int getTileSize() {return m_tileSize;}
    bool isCollision(Vector2 position, int tileType) const;
    Vector2 getSpawnPosition();

    std::vector<Edge> getEdges();
};

