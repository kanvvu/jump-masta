#include "tileLevelManager.hpp"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>


std::vector<std::string> split(std::string s, const std::string& delimiter);
std::string GetLevelPath();


TileLevelManager::TileLevelManager()
{
    #if defined(DEV_MODE) 
    std::cout<<"\t\tDEV_MODE!\n";
    #endif
    loadLevels();

    // TileMapLevel temp;
    // temp.width = 3;
    // temp.height = 4;
    // temp.name = "temp";

    // std::vector<int> tempLevel = {
    //     1,1,1,
    //     1,0,1,
    //     1,0,1,
    //     1,1,1,
    // };
    
    // temp.level = tempLevel;
    // saveLevel(temp);
}

void TileLevelManager::loadLevels()
{
  std::ifstream file(GetLevelPath());

  if (!file)
    {
        std::cerr << "Could not open "<<GetLevelPath();
        return;
    }

    int lineNumber = 1;
    std::string s;

    while (std::getline(file, s)) {
        TileMapLevel tileMap;

        std::vector<std::string> tokens;
        tokens = split(s, " ");

        if (tokens.size() != 2) {
            std::cerr << "Invalid map dimensions, should be 2, is " + tokens.size() << std::endl;
            return;
        } 
        int width = std::stoi(tokens[0]);
        int height = std::stoi(tokens[1]);

        tileMap.width = width;
        tileMap.height = height;

        if (!std::getline(file, s)) {
            std::cerr << "Invalid map file, file to short! \n";
            return;
        }
        std::string mapName = s;
        std::cout<<width<<" "<<height<<" "<<mapName<<std::endl;
        tileMap.name = mapName;

        for (int i = 0; i<height; i++) {
            if (!std::getline(file, s)) {
                std::cerr << "Invalid map file, file to short! \n";
                return;
            }

            tokens = split(s, " ");
            if (tokens.size() != width) {
                std::cerr << "Invalid map file, loaded "<<tokens.size()<<", should be "<<width<<std::endl;
                return;
            }
            
            for(std::string& token : tokens) {
                tileMap.level.push_back(stoi(token));
            }
        }
        tileMaps.push_back(tileMap);

        std::streampos originalPosition = file.tellg();

        if (!std::getline(file,s)) {
            std::cerr << "No assigned levels info!\n";
            assignedLevels.push_back(std::vector<int>());
            return;
        }

        tokens = split(s, " ");
        if (tokens.size() != 1) {
            std::cerr << "No assigned levels info! (invalid dimensions, should be 1 is "<<tokens.size()<<" )\n";
            assignedLevels.push_back(std::vector<int>());
            file.clear();
            file.seekg(originalPosition);
        } else {
            int edgesCount = stoi(tokens[0]);
            std::vector<int> assignedLevel;
            bool isOk = true;
            for(int i=0; i < edgesCount; i++) {
                if (!std::getline(file, s)) {
                    std::cerr << "Invalid level info, info to short! \n";
                    isOk = false;
                    break;
                }

                tokens = split(s, " ");
                if (tokens.size() != 1) {
                    std::cerr << "Invalid level info, assigned level should be 1 is "<<tokens.size()<<"\n";
                    isOk = false;
                    break;
                }
                assignedLevel.push_back(stoi(tokens[0]));
            }
            if (isOk) {
                assignedLevels.push_back(assignedLevel);
            } else {
                assignedLevels.push_back(std::vector<int>());
                file.clear();
                file.seekg(originalPosition);
            }
        }

    }
  file.close();
}

std::ostringstream TileLevelManager::levelToStream(const TileMapLevel& tileMap,const std::vector<Edge>& edges)
{
    std::ostringstream data;

    data << tileMap.width << " " << tileMap.height << "\n";
    data << tileMap.name << "\n";

    for (int i=0; i<tileMap.height; i++) {
        for (int j=0; j<tileMap.width; j++) {
            int index = i*tileMap.width + j;
            
            if (index >= tileMap.level.size()) {
                std::cerr << "Invalid index! out of range "<<index<<", should be <= "<< tileMap.level.size();
                data.str("");
                return data;
            }
            data << tileMap.level.at(index);
            if (j + 1 != tileMap.width) data << " ";
        }
        data << "\n";
    }
    data << edges.size() << "\n";
    for(int i=0; i<edges.size(); i++) {
        data << edges[i].assignedNextLevel;
        // if (i+1 != edges.size()) data << "\n";
        data << "\n";
    }
    
    return data; 
}

void TileLevelManager::saveLevel(const TileMapLevel& tileMap, const std::vector<Edge>& edges)
{
    
    tileMaps.push_back(tileMap);
    std::vector<int> assigned;
    for(const auto& e : edges) assigned.push_back(e.assignedNextLevel);
    assignedLevels.push_back(assigned);
    
    std::ostringstream data = levelToStream(tileMap, edges);
    
    std::ofstream file;
    file.open(GetLevelPath(), std::ios_base::app);

    file << data.str();

    file.close();

}

void TileLevelManager::saveLevels()
{
    if (assignedLevels.size() != tileMaps.size()) {
        std::cerr<<"ERROR [saveLevels] "<<assignedLevels.size()<<" != "<<tileMaps.size()<<"\n";
        return;
    }

    std::ofstream file;
    file.open(GetLevelPath());

    for(int i=0; i<assignedLevels.size(); i++) {
        std::vector<Edge> edges;
        for(const auto& e : assignedLevels.at(i)) {
            edges.push_back(Edge());
            edges.back().assignedNextLevel = e;
        }

        std::ostringstream data = levelToStream(tileMaps.at(i), edges);
        file<<data.str();
    }

    file.close();
}

void TileLevelManager::deleteLevel(int levelID)
{
    if (assignedLevels.size() <= levelID) {
        std::cerr<<"ERROR [deleteLevel] "<<assignedLevels.size()<<" <= "<<levelID<<"\n";
        return;
    }
    
    if (assignedLevels.size() != tileMaps.size()) {
        std::cerr<<"ERROR [deleteLevel] "<<assignedLevels.size()<<" != "<<tileMaps.size()<<"\n";
        return;
    }

    for (int i=0; i<assignedLevels.size(); i++) {
        if (i == levelID) continue;

        std::vector<int>& level = assignedLevels[i]; 
        for(int j=0; j<level.size(); j++) {
            if (level[j] == levelID) level[j] = -1;
            else if (level[j] > levelID && level[j] != -1) level[j]--;
        }
        
    }

    assignedLevels.erase(assignedLevels.begin() + levelID);
    tileMaps.erase(tileMaps.begin() + levelID);
}

void TileLevelManager::editLevel(int levelID, const TileMapLevel& tileMap,const std::vector<Edge>& edges)
{
    if (tileMaps.size() <= levelID) {
        std::cerr<<"ERROR [editLevel] "<<tileMaps.size()<<" <= "<<levelID<<"\n";
        return;
    }
    if (assignedLevels.size() <= levelID) {
        std::cerr<<"ERROR [editLevel] "<<assignedLevels.size()<<" <= "<<levelID<<"\n";
        return;
    }

    TileMapLevel& toEdit = tileMaps.at(levelID);
    toEdit.level.clear();

    toEdit.width = tileMap.width;
    toEdit.height = tileMap.height;
    toEdit.name = tileMap.name;

    for(const auto& i : tileMap.level) toEdit.level.push_back(i);

    std::vector<int>& assignedToEdit = assignedLevels.at(levelID);
    assignedToEdit.clear();
    for(const auto& e : edges) assignedToEdit.push_back(e.assignedNextLevel);  

}


TileMapLevel TileLevelManager::getLevel(int levelID) const
{
    if (levelID >= tileMaps.size()) {
        std::cerr << "Invalid levelID! out of range "<<levelID<<", should be <= "<< tileMaps.size();
        TileMapLevel empty = {
            {},
            0,
            0,
            "ERROR"
        };

        return empty;
    }

    return tileMaps.at(levelID);
}

int TileLevelManager::getLevelCount()
{
    return tileMaps.size();
}

std::vector<int> TileLevelManager::getAssigned(int levelID) const
{
    if (assignedLevels.size() <= levelID) {
        std::cerr<<"\033[31m"<<"ERROR invalid levelID for assignedLevels! given: "<<levelID<<", max is: "<<assignedLevels.size()-1<<"\n";
        return std::vector<int>();
    }
    return assignedLevels.at(levelID);
}

std::string GetLevelPath() {

#if defined(DEV_MODE) && !defined(__EMSCRIPTEN__)
    return std::string(SOURCE_RESOURCE_DIR) + "levels.txt";
#else
    return "resources/levels.txt";
#endif

}

std::vector<std::string> split(std::string s, const std::string& delimiter) {
    std::vector<std::string> tokens;
    size_t pos = 0;
    std::string token;
    while ((pos = s.find(delimiter)) != std::string::npos) {
        token = s.substr(0, pos);
        tokens.push_back(token);
        s.erase(0, pos + delimiter.length());
    }
    tokens.push_back(s);

    return tokens;
}