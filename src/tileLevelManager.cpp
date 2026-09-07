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
    }
  file.close();
}

void TileLevelManager::saveLevel(TileMapLevel tileMap)
{
    std::ostringstream data;

    data << "\n" << tileMap.width << " " << tileMap.height << "\n";
    data << tileMap.name << "\n";

    for (int i=0; i<tileMap.height; i++) {
        for (int j=0; j<tileMap.width; j++) {
            int index = i*tileMap.width + j;
            
            if (index >= tileMap.level.size()) {
                std::cerr << "Invalid index! out of range "<<index<<", should be <= "<< tileMap.level.size();
                return;
            }
            data << tileMap.level.at(index);
            if (j + 1 != tileMap.width) data << " ";
        }
        if (i+1 != tileMap.height) data << "\n";
    }
    

    std::ofstream file;
    file.open(GetLevelPath(), std::ios_base::app);

    file << data.str();

    file.close();
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