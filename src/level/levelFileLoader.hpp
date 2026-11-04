#pragma once
#include "app/common.hpp"
#include "levelData.hpp"

class LevelFileLoader
{
private:
public:
    LevelFileLoader() = default;
    ~LevelFileLoader() = default;

    std::ostringstream levelToStream(const LevelData& level) const;

    // LevelData loadLevel(int levelID);
    void saveLevel(const LevelData& level) const;
    
    std::vector<LevelData> loadLevels() const;
    void saveLevels(const std::vector<LevelData>& levels) const;
};

