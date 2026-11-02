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
    void loadAll();
    void saveLevel(const LevelData& levelData);

    void deleteLevel(int levelID);
    void editLevel(int levelID, const LevelData& levelData);

    LevelData& getLevel(int levelID);
    const LevelData& getLevel(int levelID) const; 
    int getLevelCount();

    std::vector<Edge> createEdges(const LevelData& levelData) const;
};

