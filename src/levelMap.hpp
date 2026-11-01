#pragma once

#include <vector>
#include "levelRepository.hpp"
#include "raylib.h"
#include "common.hpp"
#include "player.hpp"
#include "assets.hpp"

class LevelMap
{
private:
    enum CollisionSide {
        LEFT,
        RIGHT,
        TOP,
        DOWN
    };
    
    LevelRepository& levelRepository;
    Assets& assets;
    int currentTileMapIndex = 11;

    Sprite background;
    Sprite grass;
    
    public:
    LevelMap(LevelRepository& levelRepository, Assets& assets); 
    ~LevelMap() = default;
    
    void update();
    void draw();
    
    int getTileSize();
    bool isCollision(Vector2 position, int tileType) const;
    Vector2 getSpawnPosition();
    Vector2i getSize();
    
    void checkAndResolveCollision(Player& player);
    void resolveCollision(Player& player, CollisionSide collisionSide);
    bool checkForNextLevel(Vector2& playerPosition);
    void changeLevel(int levelID);
    void resetLevel() {changeLevel(currentTileMapIndex);};
};

