#pragma once

#include <vector>
#include "levelRepository.hpp"
#include "raylib.h"
#include "common.hpp"
#include "player.hpp"

class LevelMap
{
private:
    
    int m_tileSize = 64;
    LevelRepository& levelRepository;
    int currentTileMapIndex = 0;

    
    public:
    LevelMap(LevelRepository& levelRepository); 
    ~LevelMap() = default;
    
    void update();
    void draw();
    
    int getTileSize() {return m_tileSize;}
    bool isCollision(Vector2 position, int tileType) const;
    Vector2 getSpawnPosition();
    Vector2i getSize();
    
    void checkAndResolveCollision(Player& player);
    bool checkForNextLevel(Vector2& playerPosition);
    void changeLevel(int levelID);
};

