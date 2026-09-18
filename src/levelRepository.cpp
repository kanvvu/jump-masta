#include "levelRepository.hpp"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>


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


LevelData LevelRepository::getLevel(int levelID) const
{
    if (levelID >= levels.size() || levelID < 0) {
        std::cerr<<"ERROR [LevelRepository::getLevel] id : "<<levelID<<" >= "<<levels.size()<<"\n";
        return LevelData(0,0);
    }

    // std::cout<<"LEVEL: "<<levelID<<std::endl;
    // for(int i=0; i<levels[levelID].height; i++) {
    //     for(int j=0; j<levels[levelID].width; j++) {
    //         std::cout<<levels[levelID].level[i*levels[levelID].width+j]<<", ";
    //     }
    //     std::cout<<"\n";
    // }
    // std::cout<<std::endl;

    return levels.at(levelID);
}

int LevelRepository::getLevelCount()
{
    return levels.size();
}

std::vector<Edge> LevelRepository::createEdges(const LevelData& levelData) const
{
    std::vector<Edge> edges;


    int widthCount = levelData.getWidth();
    int heightCount = levelData.getHeight();

    for(int i=0; i<widthCount; i++) {
        //top
        int index = i;
        if (levelData.get(index) == 0) {
            Edge edge;
            edge.type = TOP;
            edge.assignedNextLevel = -1;
            edge.tilesCount = 1;
            edge.start = {i, 0};
            edge.end = {(i+1), 0};

            if (i != 0 && !edges.empty() && edges.back().end.y == 0 && edges.back().end.x == edge.start.x) { 
                edges.back().end.x = (i+1);
                edges.back().tilesCount++;
            }
            else
                edges.push_back(edge);
        }
    }

    for(int i=0; i<widthCount; i++) {
        //bottom
        int index = (heightCount-1)*widthCount + i;
        if (levelData.get(index) == 0) {
            Edge edge;
            edge.type = DOWN;
            edge.assignedNextLevel = -1;
            edge.tilesCount = 1;
            edge.start = {i, (heightCount)};
            edge.end = {(i+1), (heightCount)};

            if (i != 0 && !edges.empty() && edges.back().end.y == (heightCount) && edges.back().end.x == edge.start.x) {
                edges.back().end.x = (i+1);
                edges.back().tilesCount++;
            }
            else 
                edges.push_back(edge);
        }
    }

    for(int j=0; j<heightCount; j++) {
        // left
        int index = j*widthCount;
        if (levelData.get(index) == 0) {
            Edge edge;
            edge.type = LEFT;
            edge.assignedNextLevel = -1;
            edge.tilesCount = 1;
            edge.start = {0, j};
            edge.end = {0, (j+1)};

            if (j != 0 && !edges.empty() && edges.back().end.x == 0 && edges.back().end.y == edge.start.y) {
                edges.back().end.y = (j+1);
                edges.back().tilesCount++;
            }
            else 
                edges.push_back(edge);
        }
    }


    for(int j=0; j<heightCount; j++) {
        // right
        int index = j*widthCount + (widthCount-1);
        if (levelData.get(index) == 0) {
            Edge edge;
            edge.type = RIGHT;
            edge.assignedNextLevel = -1;
            edge.tilesCount = 1;
            edge.start = {widthCount, j};
            edge.end = {widthCount, (j+1)};

            if (j != 0 && !edges.empty() && edges.back().end.x == widthCount && edges.back().end.y == edge.start.y) {
                edges.back().end.y = (j+1);
                edges.back().tilesCount++;
            }
            else 
                edges.push_back(edge);
        }
    }

    return edges;
}
