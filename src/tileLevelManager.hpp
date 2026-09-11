#pragma once

#include <vector>
#include "common.hpp"

class TileLevelManager
{
private:
    std::vector<TileMapLevel> tileMaps;
    std::vector<std::vector<Edge>> allEdges;

    std::ostringstream levelToStream(const TileMapLevel& tileMap, const std::vector<Edge>& edges);
public:
    TileLevelManager();
    ~TileLevelManager() = default;

    void loadLevels();

    void saveLevel(const TileMapLevel& tileMap,const std::vector<Edge>& edges);
    void saveLevels();

    void deleteLevel(int levelID);
    void editLevel(int levelID, const TileMapLevel& tileMap,const std::vector<Edge>& edges);

    TileMapLevel getLevel(int levelID) const;
    int getLevelCount();

    std::vector<Edge> getAssigned(int levelID) const;
};

