#pragma once

#include <vector>

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
    void draw();
};

