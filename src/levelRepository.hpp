#pragma once

#include <vector>
#include "common.hpp"
#include "levelFileLoader.hpp"

class LevelRepository
{
private:
    std::vector<LevelData> levels;

    const LevelFileLoader& levelFileLoader;

public:
    LevelRepository(const LevelFileLoader& levelFileLoader);
    ~LevelRepository() = default;

    void saveAll() {levelFileLoader.saveLevels(levels);};
    void loadAll() {levels = levelFileLoader.loadLevels();};
    void saveLevel(const LevelData& levelData) {levelFileLoader.saveLevel(levelData);};

    void deleteLevel(int levelID);
    void editLevel(int levelID, const LevelData& tileMap,const std::vector<Edge>& edges);

    LevelData getLevel(int levelID) const;
    int getLevelCount();

    std::vector<Edge> createEdges(const LevelData& levelData) const;
    void changeLevelDimensions(int levelID, int width, int height);
};

