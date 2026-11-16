#include "levelFileLoader.hpp"
#include <sstream>
#include <string>
#include <fstream>
#include <iostream>

static std::string GetLevelPath() {
#if defined(DEV_MODE) && !defined(__EMSCRIPTEN__)
    return std::string(SOURCE_RESOURCE_DIR) + "levels.txt";
#else
    return "resources/levels.txt";
#endif

}

static std::vector<std::string> split(std::string s, const std::string& delimiter) {
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

std::ostringstream LevelFileLoader::levelToStream(const LevelData &level) const
{
    std::ostringstream data;

    data << level.getWidth() << " " << level.getHeight() << " " << level.getTileSize() <<"\n";
    data << level.getName() << "\n";

    for (int i=0; i<level.getHeight(); i++) {
        for (int j=0; j<level.getWidth(); j++) {
            int index = i*level.getWidth() + j;
            
            if (index >= level.getLevelSize()) {
                std::cerr <<"[LevelFileLoader::loadLevels] " << "Invalid index! out of range "<<index<<", should be <= "<< level.getLevelSize();
                data.str("");
                return data;
            }
            data << level.get(index);
            if (j + 1 != level.getWidth()) data << " ";
        }
        data << "\n";
    }
    data << level.getEdgesSize() << "\n";
    for(int i=0; i<level.getEdgesSize(); i++) {
        data << level.getEdge(i).assignedNextLevel;
        data << "\n";
    }
    
    return data; 
}

void LevelFileLoader::saveLevel(const LevelData &level) const
{
    std::ostringstream data = levelToStream(level);
    
    std::ofstream file;
    file.open(GetLevelPath(), std::ios_base::app);

    file << data.str();

    file.close();
}

std::vector<LevelData> LevelFileLoader::loadLevels() const
{

    std::vector<LevelData> levels;
    std::vector<LevelData> empty;

    std::ifstream file(GetLevelPath());

    if (!file)
        {
            std::cerr << "[LevelFileLoader::loadLevels] Could not open "<<GetLevelPath();
            return empty;
        }

        std::string s;

        while (std::getline(file, s)) {

            std::vector<std::string> tokens;
            tokens = split(s, " ");

            if (!(tokens.size() == 2 || tokens.size() == 3)) {
                std::cerr <<"[LevelFileLoader::loadLevels] " <<"Invalid map dimensions, should be 2 or 3, is " << tokens.size() << std::endl;
                return empty;
            } 
            int width = std::stoi(tokens[0]);
            int height = std::stoi(tokens[1]);
            int tileSize = 64;
            if (tokens.size() == 3) tileSize = std::stoi(tokens[2]); 

            
            if (!std::getline(file, s)) {
                std::cerr <<"[LevelFileLoader::loadLevels] " << "Invalid map file, file to short! \n";
                return empty;
            }
            std::string levelName = s;
        
            LevelData levelData(width, height, levelName, tileSize);

            for (int i = 0; i<height; i++) {
                if (!std::getline(file, s)) {
                    std::cerr <<"[LevelFileLoader::loadLevels] " << "Invalid map file, file to short! \n";
                    return empty;
                }

                tokens = split(s, " ");
                if (tokens.size() != width) {
                    std::cerr <<"[LevelFileLoader::loadLevels] " << "Invalid map file, loaded "<<tokens.size()<<", should be "<<width<<std::endl;
                    return empty;
                }
                
                for(int t=0; t<tokens.size(); t++) {
                    std::string& token = tokens[t];
                    levelData.set(t, i, stoi(token));
                }
            }

            std::streampos originalPosition = file.tellg();

            if (!std::getline(file,s)) {
                std::cerr <<"[LevelFileLoader::loadLevels] " << "No assigned levels info!\n";
                return empty;
            }

            tokens = split(s, " ");
            if (tokens.size() != 1) {
                std::cerr <<"[LevelFileLoader::loadLevels] " << "No assigned levels info! (invalid dimensions, should be 1 is "<<tokens.size()<<" )\n";
                file.clear();
                file.seekg(originalPosition);
            } else {
                int edgesCount = stoi(tokens[0]);
                std::vector<Edge> edges;
                bool isOk = true;
                for(int i=0; i < edgesCount; i++) {
                    if (!std::getline(file, s)) {
                        std::cerr <<"[LevelFileLoader::loadLevels] " << "Invalid level info, info to short! \n";
                        isOk = false;
                        break;
                    }

                    tokens = split(s, " ");
                    if (tokens.size() != 1) {
                        std::cerr <<"[LevelFileLoadeLevelFileLoader::loadLevels] " << "Invalid level info, assigned level should be 1 is "<<tokens.size()<<"\n";
                        isOk = false;
                        break;
                    }

                    Edge edge;
                    edge.assignedNextLevel = stoi(tokens[0]);
                    edges.push_back(edge);
                }
                if (isOk) {
                    levelData.setEdges(edges);
                } else {
                    file.clear();
                    file.seekg(originalPosition);
                }
            }

            levels.push_back(levelData);
        }
    file.close();

    return levels;
}

void LevelFileLoader::saveLevels(const std::vector<LevelData> &levels) const
{
    std::ofstream file;
    file.open(GetLevelPath());

    for(int i=0; i<levels.size(); i++) {
        std::ostringstream data = levelToStream(levels[i]);
        file<<data.str();
    }

    file.close();
}
