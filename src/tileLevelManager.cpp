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
        std::cerr << "[TileLevelManager::loadLevels] Could not open "<<GetLevelPath();
        return;
    }

    int lineNumber = 1;
    std::string s;

    while (std::getline(file, s)) {
        TileMapLevel tileMap;

        std::vector<std::string> tokens;
        tokens = split(s, " ");

        if (tokens.size() != 2) {
            std::cerr <<"[TileLevelManager::loadLevels] " <<"Invalid map dimensions, should be 2, is " + tokens.size() << std::endl;
            return;
        } 
        int width = std::stoi(tokens[0]);
        int height = std::stoi(tokens[1]);

        tileMap.width = width;
        tileMap.height = height;

        if (!std::getline(file, s)) {
            std::cerr <<"[TileLevelManager::loadLevels] " << "Invalid map file, file to short! \n";
            return;
        }
        std::string mapName = s;
        std::cout<<width<<" "<<height<<" "<<mapName<<std::endl;
        tileMap.name = mapName;

        for (int i = 0; i<height; i++) {
            if (!std::getline(file, s)) {
                std::cerr <<"[TileLevelManager::loadLevels] " << "Invalid map file, file to short! \n";
                return;
            }

            tokens = split(s, " ");
            if (tokens.size() != width) {
                std::cerr <<"[TileLevelManager::loadLevels] " << "Invalid map file, loaded "<<tokens.size()<<", should be "<<width<<std::endl;
                return;
            }
            
            for(std::string& token : tokens) {
                tileMap.level.push_back(stoi(token));
            }
        }
        tileMaps.push_back(tileMap);

        std::streampos originalPosition = file.tellg();

        if (!std::getline(file,s)) {
            std::cerr <<"[TileLevelManager::loadLevels] " << "No assigned levels info!\n";
            allEdges.push_back(std::vector<Edge>());
            return;
        }

        tokens = split(s, " ");
        if (tokens.size() != 1) {
            std::cerr <<"[TileLevelManager::loadLevels] " << "No assigned levels info! (invalid dimensions, should be 1 is "<<tokens.size()<<" )\n";
            allEdges.push_back(std::vector<Edge>());
            file.clear();
            file.seekg(originalPosition);
        } else {
            int edgesCount = stoi(tokens[0]);
            std::vector<Edge> edges;
            bool isOk = true;
            for(int i=0; i < edgesCount; i++) {
                if (!std::getline(file, s)) {
                    std::cerr <<"[TileLevelManager::loadLevels] " << "Invalid level info, info to short! \n";
                    isOk = false;
                    break;
                }

                tokens = split(s, " ");
                if (tokens.size() != 1) {
                    std::cerr <<"[TileLevelManager::loadLevels] " << "Invalid level info, assigned level should be 1 is "<<tokens.size()<<"\n";
                    isOk = false;
                    break;
                }

                Edge edge;
                edge.assignedNextLevel = stoi(tokens[0]);
                edges.push_back(edge);
            }
            if (isOk) {
                allEdges.push_back(edges);
            } else {
                allEdges.push_back(std::vector<Edge>());
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
                std::cerr <<"[TileLevelManager::loadLevels] " << "Invalid index! out of range "<<index<<", should be <= "<< tileMap.level.size();
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
    allEdges.push_back(edges);
    
    std::ostringstream data = levelToStream(tileMap, edges);
    
    std::ofstream file;
    file.open(GetLevelPath(), std::ios_base::app);

    file << data.str();

    file.close();

}

void TileLevelManager::saveLevels()
{
    if (allEdges.size() != tileMaps.size()) {
        std::cerr<<"ERROR [TileLevelManager::saveLevels] "<<allEdges.size()<<" != "<<tileMaps.size()<<"\n";
        return;
    }

    std::ofstream file;
    file.open(GetLevelPath());

    for(int i=0; i<allEdges.size(); i++) {
        std::ostringstream data = levelToStream(tileMaps.at(i), allEdges.at(i));
        file<<data.str();
    }

    file.close();
}

void TileLevelManager::deleteLevel(int levelID)
{
    if (allEdges.size() <= levelID) {
        std::cerr<<"ERROR [TileLevelManager::deleteLevel] "<<allEdges.size()<<" <= "<<levelID<<"\n";
        return;
    }
    
    if (allEdges.size() != tileMaps.size()) {
        std::cerr<<"ERROR [TileLevelManager::deleteLevel] "<<allEdges.size()<<" != "<<tileMaps.size()<<"\n";
        return;
    }

    for (int i=0; i<allEdges.size(); i++) {
        if (i == levelID) continue;

        std::vector<Edge>& edges = allEdges.at(i);
        for(int j=0; j<edges.size(); j++) {
            if (edges[j].assignedNextLevel == levelID) edges[j].assignedNextLevel = -1;
            else if (edges[j].assignedNextLevel > levelID && edges[j].assignedNextLevel != -1) edges[j].assignedNextLevel--;
        }
        
    }

    allEdges.erase(allEdges.begin() + levelID);
    tileMaps.erase(tileMaps.begin() + levelID);
}

void TileLevelManager::editLevel(int levelID, const TileMapLevel& tileMap,const std::vector<Edge>& edges)
{
    if (tileMaps.size() <= levelID) {
        std::cerr<<"ERROR [TileLevelManager::editLevel] "<<tileMaps.size()<<" <= "<<levelID<<"\n";
        return;
    }
    if (allEdges.size() <= levelID) {
        std::cerr<<"ERROR [TileLevelManager::editLevel] "<<allEdges.size()<<" <= "<<levelID<<"\n";
        return;
    }

    TileMapLevel& toEdit = tileMaps.at(levelID);
    toEdit.level.clear();

    toEdit.width = tileMap.width;
    toEdit.height = tileMap.height;
    toEdit.name = tileMap.name;

    for(const auto& i : tileMap.level) toEdit.level.push_back(i);

    std::vector<Edge>& assignedToEdit = allEdges.at(levelID);
    assignedToEdit.clear();
    assignedToEdit = edges;

}


TileMapLevel TileLevelManager::getLevel(int levelID) const
{
    if (levelID >= tileMaps.size()) {
        std::cerr << "[TileMapLevel::getLevel] Invalid levelID! out of range "<<levelID<<", should be <= "<< tileMaps.size()<<"\n";
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

std::vector<Edge> TileLevelManager::getEdges(int levelID) const
{
    if (allEdges.size() <= levelID) {
        std::cerr<<"\033[31m"<<"[TileMapLevel::getLevel] "<<"ERROR invalid levelID for assignedLevels! given: "<<levelID<<", should be <= "<<allEdges.size()<<"\033[0m\n";
        return std::vector<Edge>();
    }
    return allEdges.at(levelID);
}


std::vector<Edge> TileLevelManager::createEdges(const std::vector<int>& tileMap, int tileCountX,int tileCountY) const
{
    std::vector<Edge> edges;


    int widthCount = tileCountX;
    int heightCount = tileCountY;

    for(int i=0; i<widthCount; i++) {
        //top
        int index = i;
        if (tileMap.at(index) == 0) {
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
        if (tileMap.at(index) == 0) {
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
        if (tileMap.at(index) == 0) {
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
        if (tileMap.at(index) == 0) {
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