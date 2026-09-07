#pragma once

#include <vector>
#include "common.hpp"

class TileLevelManager
{
private:
    std::vector<TileMapLevel> tileMaps;

public:
    TileLevelManager();
    ~TileLevelManager() = default;

    void loadLevels();

    void saveLevel(TileMapLevel tileMap);
    TileMapLevel getLevel(int levelID) const;
    int getLevelCount();
};

