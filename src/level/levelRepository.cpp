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
    // levels = levelFileLoader.loadLevels();
    loadAll();
    // std::cout<<"LEVEL: "<<0<<std::endl;
    // LevelData& l = levels[0];
    // for(int i=0; i<l.height; i++) {
    //     for(int j=0; j<l.width; j++) {
    //         std::cout<<l.level[i*l.width+j]<<", ";
    //     }
    //     std::cout<<"\n";
    // }
    // std::cout<<std::endl;
}

void LevelRepository::loadAll()
{
    levels = levelFileLoader.loadLevels();

    // int i=0;

    // for(auto& level : levels) {
    //     std::vector<Edge> assignedEdges = level.edges;
    //     level.edges = createEdges(level);
    //     std::cout<<"LEVEL: "<<i<<", assigned: "<<assignedEdges.size()<<", created: "<<level.edges.size()<<"\n";

    //     for(int i=0; i<level.edges.size(); i++) {
    //         if (i >= assignedEdges.size()) continue;
    //         level.edges[i].assignedNextLevel = assignedEdges[i].assignedNextLevel;
    //     }
    // }
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

        // std::vector<Edge>& edges = levels[i].edges;
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
    assert(levelID >= 0 && levelID < (int)levels.size() && "getLevel: invalid id");

    return levels.at(levelID);
}

const LevelData& LevelRepository::getLevel(int levelID) const
{
    assert(levelID >= 0 && levelID < (int)levels.size() && "getLevel: invalid id");

    return levels.at(levelID);
}

int LevelRepository::getLevelCount()
{
    return levels.size();
}
