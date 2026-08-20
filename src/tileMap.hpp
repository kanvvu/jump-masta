#pragma once

#include <vector>
#include "raylib.h"

class TileMap
{
private:
    const int m_widthCount;
    const int m_heightCount;
    const int m_tileSize = 64;

    std::vector<std::vector<int>> tileMaps;
    int currentTileMapIndex = 0;

public:
    TileMap(int widthCount, int heightCount);
    ~TileMap() = default;

    void update();
    void draw(Vector2 offset);

    int getTileSize() {return m_tileSize;}
    bool isCollision(Vector2 position, int tileType) const;
    Vector2 getSpawnPosition();
};

