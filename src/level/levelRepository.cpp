#include "levelRepository.hpp"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include <cassert>


std::vector<std::string> split(std::string s, const std::string& delimiter);
std::string GetLevelPath();


LevelRepository::LevelRepository(const LevelFileLoader& levelFileLoader) : levelFileLoader(levelFileLoader)
{
    #if defined(DEV_MODE) 
    std::cout<<"\t\tDEV_MODE!\n";
    #endif
    loadAll();
}

void LevelRepository::loadAll()
{
    levels = levelFileLoader.loadLevels();
}

void LevelRepository::saveLevel(const LevelData &levelData)
{
    //assume that edges are provided
    levels.push_back(levelData);
    levelFileLoader.saveLevel(levelData);
}

void LevelRepository::deleteLevel(int levelID)
{
    if (levelID >= levels.size() || levelID < 0) {
        std::cerr<<"ERROR [LevelRepository::deleteLevel] id : "<<levelID<<" >= "<<levels.size()<<"\n";
        return;
    }

    for (int i=0; i<levels.size(); i++) {
        if (i == levelID) continue;

        LevelData& level = levels[i];
        for(int j=0; j<level.getEdgesSize(); j++) {
            if (level.getEdge(j).assignedNextLevel == levelID) level.getEdge(j).assignedNextLevel = -1;
            else if (level.getEdge(j).assignedNextLevel > levelID && level.getEdge(j).assignedNextLevel != -1) level.getEdge(j).assignedNextLevel--;
        }
        
    }

    levels.erase(levels.begin() + levelID);
}

void LevelRepository::editLevel(int levelID, const LevelData& levelData)
{
    if (levelID >= levels.size() || levelID < 0) {
        std::cerr<<"ERROR [LevelRepository::editLevel] id : "<<levelID<<" >= "<<levels.size()<<"\n";
        return;
    }

    levels.at(levelID) = levelData;
}


LevelData& LevelRepository::getLevel(int levelID)
{
    assert(levelID >= 0 && levelID < static_cast<int>(levels.size()) && "getLevel: invalid id");

    return levels.at(levelID);
}

const LevelData& LevelRepository::getLevel(int levelID) const
{
    assert(levelID >= 0 && levelID < static_cast<int>(levels.size()) && "getLevel: invalid id");

    return levels.at(levelID);
}

int LevelRepository::getLevelCount()
{
    return static_cast<int>(levels.size());
}
