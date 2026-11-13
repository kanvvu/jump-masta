#pragma once

#include <vector>
#include "levelRepository.hpp"
#include "raylib.h"
#include "app/common.hpp"
#include "game/player.hpp"
#include "assets/assets.hpp"
#include <optional>


void drawLevel(const LevelData &level, Assets &assets, const SpriteGroup& background, bool debugInfo=false);

class LevelMap
{
private:
    enum class CollisionSide {
        LEFT,
        RIGHT,
        TOP,
        DOWN
    };

    static constexpr int startLevelIndex = 11;
    
    LevelRepository& levelRepository;
    Assets& assets;
    int currentTileMapIndex;

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
    std::optional<Vector2> getGoalPosition();
    Vector2i getSize();
    int getCurrentLevelIndex() const {return currentTileMapIndex;}
    int getStartLevelIndex() const {return startLevelIndex;}

    bool isGoalReached(Vector2 playerPosition) const;
    
    void checkAndResolveCollision(Player& player);
    void resolveCollision(Player& player, CollisionSide collisionSide);
    bool checkForNextLevel(Vector2& playerPosition);
    void changeLevel(int levelID);
    void resetLevel() {changeLevel(currentTileMapIndex);};
};

