#pragma once

#include <vector>
#include "levelRepository.hpp"
#include "raylib.h"
#include "app/common.hpp"
#include "game/player.hpp"
#include "assets/assets.hpp"


void drawLevel(const LevelData& level, Assets& assets);

class LevelMap
{
private:
    enum class CollisionSide {
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
    void drawBackground();
    void draw();
    
    int getTileSize();
    bool isCollision(Vector2 position, CellType cellType = CellType::Solid) const;
    Vector2 getSpawnPosition();
    Vector2i getSize();
    int getCurrentLevelIndex() const {return currentTileMapIndex;}
    
    void checkAndResolveCollision(Player& player);
    void resolveCollision(Player& player, CollisionSide collisionSide);
    bool checkForNextLevel(Vector2& playerPosition);
    void changeLevel(int levelID);
    void resetLevel() {changeLevel(currentTileMapIndex);};
};

