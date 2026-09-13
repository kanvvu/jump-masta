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
    levels = levelFileLoader.loadLevels();
}

void LevelRepository::deleteLevel(int levelID)
{
    if (levelID >= levels.size() || levelID < 0) {
        std::cerr<<"ERROR [LevelRepository::deleteLevel] id : "<<levelID<<" >= "<<levels.size()<<"\n";
        return;
    }

    for (int i=0; i<levels.size(); i++) {
        if (i == levelID) continue;

        std::vector<Edge>& edges = levels[i].edges;
        for(int j=0; j<edges.size(); j++) {
            if (edges[j].assignedNextLevel == levelID) edges[j].assignedNextLevel = -1;
            else if (edges[j].assignedNextLevel > levelID && edges[j].assignedNextLevel != -1) edges[j].assignedNextLevel--;
        }
        
    }

    levels.erase(levels.begin() + levelID);
}

void LevelRepository::editLevel(int levelID, const LevelData& tileMap,const std::vector<Edge>& edges)
{
    if (levelID >= levels.size() || levelID < 0) {
        std::cerr<<"ERROR [LevelRepository::editLevel] id : "<<levelID<<" >= "<<levels.size()<<"\n";
        return;
    }

    LevelData& toEdit = levels.at(levelID);
    toEdit.level.clear();

    toEdit.width = tileMap.width;
    toEdit.height = tileMap.height;
    toEdit.name = tileMap.name;

    for(const auto& i : tileMap.level) toEdit.level.push_back(i);

    std::vector<Edge>& assignedToEdit = levels.at(levelID).edges;
    assignedToEdit.clear();
    assignedToEdit = edges;

}


LevelData LevelRepository::getLevel(int levelID) const
{
    if (levelID >= levels.size() || levelID < 0) {
        std::cerr<<"ERROR [LevelRepository::getLevel] id : "<<levelID<<" >= "<<levels.size()<<"\n";
        return LevelData();
    }

    return levels.at(levelID);
}

int LevelRepository::getLevelCount()
{
    return levels.size();
}

std::vector<Edge> LevelRepository::createEdges(const LevelData& levelData) const
{
    std::vector<Edge> edges;


    int widthCount = levelData.width;
    int heightCount = levelData.height;

    for(int i=0; i<widthCount; i++) {
        //top
        int index = i;
        if (levelData.level.at(index) == 0) {
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
        if (levelData.level.at(index) == 0) {
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
        if (levelData.level.at(index) == 0) {
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
        if (levelData.level.at(index) == 0) {
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

void LevelRepository::changeLevelDimensions(int levelID, int width, int height)
{
    if (levelID >= levels.size() || levelID < 0) {
        std::cerr<<"ERROR [LevelRepository::changeLevelDimensions] id : "<<levelID<<" >= "<<levels.size()<<"\n";
        return; 
    }

    LevelData& levelToEdit = levels.at(levelID);

    std::vector<int> newLevel = std::vector<int>(width*height, 0);

    int minWidth = width > levelToEdit.width ? levelToEdit.width : width;
    int minHeight = height > levelToEdit.height ? levelToEdit.height : height;

    for(int i=0; i<minHeight; i++) {
        for(int j=0; j<minWidth; j++) {
            int index = i*width + j;
            if (index >= newLevel.size()) {
                std::cerr<<"[TileLevelManager::changeLevelDimensions] wrong index: "<<index<<", should be "<<newLevel.size()<<"\n";
                return;
            }
            int index2 = i*levelToEdit.width + j;
            if (index2 >= levelToEdit.level.size()) {
                std::cerr<<"[TileLevelManager::changeLevelDimensions] wrong index: "<<index2<<", should be "<<levelToEdit.level.size()<<"\n";
                return;
            }
            newLevel[index] = levelToEdit.level[index2];
        }
    }

    levelToEdit.level = newLevel;
    levelToEdit.width = width;
    levelToEdit.height = height;
}