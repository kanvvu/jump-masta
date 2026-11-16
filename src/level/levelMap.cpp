#include "levelMap.hpp"
#include <iostream>
#include "raylib.h"
#include "raymath.h"


LevelMap::LevelMap(LevelRepository& levelRepository, Assets& assets) 
    : levelRepository(levelRepository),
    assets(assets),
    background(assets.getBackground()),
    grass(assets.getGrass())
{
    currentTileMapIndex = startLevelIndex;
}

void LevelMap::changeLevel(int levelID)
{
    if (levelID < 0 || levelID >= levelRepository.getLevelCount()) {
        std::cout<<"ERROR [TileMap::changeLevel] levelID is "<<levelID<<" where max: "<<levelRepository.getLevelCount()<<"\n";
        return;
    }

    currentTileMapIndex = levelID;
}

void LevelMap::drawBackground()
{
}

void LevelMap::draw()
{
    drawLevel(levelRepository.getLevel(currentTileMapIndex), assets, assets.getBackgroundGroup());
}

int LevelMap::getTileSize()
{
    return levelRepository.getLevel(currentTileMapIndex).getTileSize();
}

bool LevelMap::isCollision(Vector2 position, CellType cellType) const
{
    int m_widthCount = levelRepository.getLevel(currentTileMapIndex).getWidth();
    int m_heightCount = levelRepository.getLevel(currentTileMapIndex).getHeight();

    int posX = static_cast<int>(position.x);
    int posY = static_cast<int>(position.y);
    if (posX >= m_widthCount|| posX < 0 || posY >= m_heightCount || posY < 0) {
        return false;
    }
    int tileId = posY*m_widthCount + posX;
    const LevelData& level = levelRepository.getLevel(currentTileMapIndex);
    
    if (level.getTileCollision(tileId) == cellType) return true;
    
    return false;
}

Vector2 LevelMap::getSpawnPosition()
{
    int m_widthCount = levelRepository.getLevel(currentTileMapIndex).getWidth();
    int m_heightCount = levelRepository.getLevel(currentTileMapIndex).getHeight();

    for(int i=0; i<m_heightCount; i++) {
        for(int j=0; j<m_widthCount; j++) {
            int tileIndex = i*m_widthCount+ j;

            CellType cellType = levelRepository.getLevel(currentTileMapIndex).getTileCollision(tileIndex);
            if (cellType == CellType::Spawn) {
                return {static_cast<float>(j), static_cast<float>(i)};
            }
        }
    }

    return {0,0};
}

std::optional<Vector2> LevelMap::getGoalPosition()
{
    int m_widthCount = levelRepository.getLevel(currentTileMapIndex).getWidth();
    int m_heightCount = levelRepository.getLevel(currentTileMapIndex).getHeight();

    std::optional<Vector2> position;

    for(int i=0; i<m_heightCount; i++) {
        for(int j=0; j<m_widthCount; j++) {
            int tileIndex = i*m_widthCount+ j;

            CellType cellType = levelRepository.getLevel(currentTileMapIndex).getTileCollision(tileIndex);
            if (cellType == CellType::Goal) {
                position = std::make_optional<Vector2>({static_cast<float>(j), static_cast<float>(i)});
                return position;
            }
        }
    }
    return position;
}

Vector2i LevelMap::getSize()
{
    return {levelRepository.getLevel(currentTileMapIndex).getWidth(), levelRepository.getLevel(currentTileMapIndex).getHeight()};
}

bool LevelMap::checkForNextLevel(Vector2& playerPosition)
{
    if (currentTileMapIndex >= levelRepository.getLevelCount() || currentTileMapIndex < 0) {
        std::cerr<<"ERROR [LevelMap::checkForNextLevel] id: "<<currentTileMapIndex<<" >= "<<levelRepository.getLevelCount()<<"\n";
        return false;
    }

    LevelData& levelData = levelRepository.getLevel(currentTileMapIndex);

    if (levelData.getEdgesSize() == 0) return false;

    for(int i=0; i<levelData.getEdgesSize(); i++) {
        const Edge& edge = levelData.getEdge(i);

        if (edge.assignedNextLevel == -1) continue;
        int m_widthCount = levelRepository.getLevel(edge.assignedNextLevel).getWidth();
        int m_heightCount = levelRepository.getLevel(edge.assignedNextLevel).getHeight();

        switch (edge.type)
        {
        case EdgeType::TOP:
            if (playerPosition.y + 0.5f < edge.start.y) {
                changeLevel(edge.assignedNextLevel);
                playerPosition.y = m_heightCount - 1.0f;
                return true;
            }
            break;
        case EdgeType::DOWN:
            if (playerPosition.y + 0.5f> edge.start.y) {
                changeLevel(edge.assignedNextLevel);
                playerPosition.y = 0.0f;
                return true;
            }
            break;
        case EdgeType::LEFT:
            if (playerPosition.x + 0.5f < edge.start.x) {
                changeLevel(edge.assignedNextLevel);
                playerPosition.x = m_widthCount - 1.0f;
                return true;
            }
            break;
        case EdgeType::RIGHT:
            if (playerPosition.x + 0.5f > edge.start.x) {
                changeLevel(edge.assignedNextLevel);
                playerPosition.x = 0.0f;
                return true;
            }
            break;
        }
    }

    return false;

}

bool LevelMap::isGoalReached(Vector2 playerPosition) const
{
    if (isCollision({playerPosition.x + 1.0f, playerPosition.y + 1.0f}, CellType::Goal) ||
           isCollision({playerPosition.x, playerPosition.y}, CellType::Goal) ||
           isCollision({playerPosition.x + 1.0f, playerPosition.y}, CellType::Goal) ||
           isCollision({playerPosition.x, playerPosition.y + 1.0f}, CellType::Goal) || 
           isCollision({playerPosition.x + 1.9f, playerPosition.y + 1.0f}, CellType::Goal) || 
           isCollision({playerPosition.x + 1.9f, playerPosition.y}, CellType::Goal) ||  
           isCollision({playerPosition.x, playerPosition.y + 1.9f}, CellType::Goal) ||  
           isCollision({playerPosition.x + 1.0f, playerPosition.y + 1.9f}, CellType::Goal) ||  
           isCollision({playerPosition.x + 1.9f, playerPosition.y + 1.9f}, CellType::Goal)) {
            return true;
           }

    return false;

}

void LevelMap::checkAndResolveCollision(Player &player)
{
    Vector2 bP = player.m_previousPosition; 
    Vector2 nBP = player.m_position;

    Vector2 movementDirection = Vector2Subtract(nBP, bP);
    if (movementDirection.x < 0) { //left

        Vector2 leftTop = {nBP.x, bP.y};
        Vector2 left = {nBP.x, bP.y + 1.0f};
        Vector2 leftDown = {nBP.x, bP.y + 1.99f};

        if (isCollision({nBP.x, bP.y}) || isCollision({nBP.x, bP.y + 1.0f}) ||
            isCollision({nBP.x, bP.y + 1.99f})) {
            nBP.x = static_cast<float>(static_cast<int>(nBP.x) + 1);
            player.m_velocity.x = -0.5f * player.m_velocity.x;
            player.m_hitWall = true;
            }
    }
    if (movementDirection.x > 0) { //right

        Vector2 rightTop = {nBP.x + 2.0f, bP.y};
        Vector2 right = {nBP.x + 2.0f, bP.y + 1.0f};
        Vector2 rightDown = {nBP.x + 2.0f, bP.y + 1.99f};

        if (isCollision({nBP.x + 2.0f, bP.y}) || isCollision({nBP.x + 2.0f, bP.y + 1.0f}) ||
            isCollision({nBP.x + 2.0f, bP.y + 1.99f})) {
            nBP.x = static_cast<float>(static_cast<int>(nBP.x));
            player.m_velocity.x = -0.5f * player.m_velocity.x;
            player.m_hitWall = true;
        }
    }

    player.onGround = false;
    if (movementDirection.y < 0) { //up

        Vector2 topLeft = {nBP.x, nBP.y};
        Vector2 topMid = {nBP.x + 1.0f, nBP.y};
        Vector2 topRight = {nBP.x + 1.90f, nBP.y};

        if (isCollision({nBP.x, nBP.y}) || isCollision({nBP.x + 1.0f, nBP.y}) ||
            isCollision({nBP.x + 1.90f, nBP.y})) {
            nBP.y = static_cast<float>(static_cast<int>(nBP.y) + 1);
            player.m_velocity.y = -0.5f * player.m_velocity.y;
            player.m_hitWall = true;
        }
    }
    else { //down

        Vector2 downLeft = {nBP.x, nBP.y + 2.0f};
        Vector2 downMid = {nBP.x + 1.0f, nBP.y + 2.0f};
        Vector2 downRight = {nBP.x + 1.90f, nBP.y + 2.0f};

        if (isCollision({nBP.x, nBP.y + 2.0f}) || isCollision({nBP.x + 1.0f, nBP.y + 2.0f}) ||
            isCollision({nBP.x + 1.90f, nBP.y + 2.0f})) {
            nBP.y = static_cast<float>(static_cast<int>(nBP.y));
            player.m_velocity.y = 0;
            player.onGround = true;
        }
    }
    player.m_position = nBP;
}

void drawLevel(const LevelData &level, Assets &assets, const SpriteGroup& background, bool debugInfo)
{
    int widthCount = level.getWidth();
    int heightCount = level.getHeight();
    int tileSize = level.getTileSize(); 

    for(int i=0; i<heightCount; i++) {
        for(int j=0; j<widthCount; j++) {
            int xPos = j*tileSize;
            int yPos = i*tileSize;

            if (i%background.xCount == 0 && j%background.yCount == 0)
                background.draw({static_cast<float>(xPos), static_cast<float>(yPos)}, {static_cast<float>(tileSize), static_cast<float>(tileSize)}, {static_cast<float>(widthCount*tileSize), static_cast<float>(heightCount*tileSize)});
        }
    }

    for(int i=0; i<heightCount; i++) {
        for(int j=0; j<widthCount; j++) {
            int tileIndex = i*widthCount+ j;
            int xPos = j*tileSize;
            int yPos = i*tileSize;
            Color tileColor = WHITE;

            int tile = level.get(tileIndex);
            CellType cellType = level.getTileCollision(tileIndex);
            if (tile != -1 ) {
                if (debugInfo || cellType != CellType::Spawn)
                    assets.getSprite(tile).draw({static_cast<float>(xPos), static_cast<float>(yPos)}, {static_cast<float>(tileSize), static_cast<float>(tileSize)});
            }
        }
    }
}