#pragma once

#include <vector>
#include "common.hpp"

class TileLevelManager
{
private:
    std::vector<TileMapLevel> tileMaps;
    std::vector<std::vector<int>> assignedLevels;

public:
    TileLevelManager();
    ~TileLevelManager() = default;

    void loadLevels();

    void saveLevel(TileMapLevel tileMap, std::vector<Edge> edges);
    TileMapLevel getLevel(int levelID) const;
    int getLevelCount();

    std::vector<int> getAssigned(int levelID) const;
};

