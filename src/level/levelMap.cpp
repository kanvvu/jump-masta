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

    int posX = position.x;
    int posY = position.y;
    if (posX >= m_widthCount|| posX < 0 || posY >= m_heightCount || posY < 0) {
        // printf("COLLSION POSITION OUT OF BOUNDS!\n");
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
                return {(float) j,(float) i};
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
                position = std::make_optional<Vector2>({(float) j,(float) i});
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

    int m_widthCount = levelData.getWidth();
    int m_heightCount = levelData.getHeight();

    if (levelData.getEdgesSize() == 0) return false;

    for(int i=0; i<levelData.getEdgesSize(); i++) {
        const Edge& edge = levelData.getEdge(i);

        if (edge.assignedNextLevel == -1) continue;
        std::cout<<playerPosition.y<<" "<<edge.start.y<<"\n";
        int m_widthCount = levelRepository.getLevel(edge.assignedNextLevel).getWidth();
        int m_heightCount = levelRepository.getLevel(edge.assignedNextLevel).getHeight();

        switch (edge.type)
        {
        case EdgeType::TOP:
            if (playerPosition.y + 0.5f < edge.start.y) {
                std::cout<<"EY JOL\n";
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

// bool LevelMap::checkForNextLevel(Vector2& playerPosition)
// {
//     if (currentTileMapIndex >= levelRepository.getLevelCount() || currentTileMapIndex < 0) {
//         std::cerr<<"ERROR [LevelMap::checkForNextLevel] id: "<<currentTileMapIndex<<" >= "<<levelRepository.getLevelCount()<<"\n";
//         return false;
//     }

//     LevelData& levelData = levelRepository.getLevel(currentTileMapIndex);

//     int m_widthCount = levelData.getWidth();
//     int m_heightCount = levelData.getHeight();

//     if (levelData.getEdgesSize() == 0) return false;

//     const std::vector<Edge>& assignedEdges = levelData.edges;
//     std::vector<Edge> edges = levelRepository.createEdges(levelData);

//     if (levelData.getEdgeSize() != edges.size()) {
//         std::cerr<<"ERROR [LevelMap::checkForNextLevel] "<<assignedEdges.size()<<" != "<<edges.size()<<std::endl;
//         return false;
//     }

//     for(int i=0; i<edges.size(); i++) {
//         const Edge& aEdge = assignedEdges.at(i);
//         const Edge& edge = edges.at(i);

//         if (aEdge.assignedNextLevel == -1) continue;
//         // std::cout<<playerPosition.y<<" "<<edge.start.y<<std::endl;
//         int m_widthCount = levelRepository.getLevel(aEdge.assignedNextLevel).width;
//         int m_heightCount = levelRepository.getLevel(aEdge.assignedNextLevel).height;

//         switch (edge.type)
//         {
//         case TOP:
//             if (playerPosition.y + 0.5f < edge.start.y) {
//                 changeLevel(aEdge.assignedNextLevel);
//                 playerPosition.y = m_heightCount - 1.0f;
//                 return true;
//             }
//             break;
//         case DOWN:
//             if (playerPosition.y + 0.5f> edge.start.y) {
//                 changeLevel(aEdge.assignedNextLevel);
//                 playerPosition.y = 0.0f;
//                 return true;
//             }
//             break;
//         case LEFT:
//             if (playerPosition.x + 0.5f < edge.start.x) {
//                 changeLevel(aEdge.assignedNextLevel);
//                 playerPosition.x = m_widthCount - 1.0f;
//                 return true;
//             }
//             break;
//         case RIGHT:
//             if (playerPosition.x + 0.5f > edge.start.x) {
//                 changeLevel(aEdge.assignedNextLevel);
//                 playerPosition.x = 0.0f;
//                 return true;
//             }
//             break;
//         }
//     }

//     return false;

// }

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
            // std::cout<<"POS: ("<<playerPosition.x<<" "<<playerPosition.y<<") ("<<(int) playerPosition.x<<" "<<(int) playerPosition.y<<") | ";
            // std::cout<<"POS: ("<<playerPosition.x+1.0f<<" "<<playerPosition.y+ 1.0f<<") ("<<(int) (playerPosition.x + 1.0f)<<" "<<(int) (playerPosition.y+1.0f)<<") \n";
            return true;
           }

    return false;

}

void LevelMap::checkAndResolveCollision(Player &player)
{
    Vector2 bP = player.m_previousPosition; 
    Vector2 nBP = player.m_position;

    //if player has more tiles then we need to check more points.
    //the points are going in for directins left, right, up and down




    
    Vector2 movementDirection = Vector2Subtract(nBP, bP);
    if (movementDirection.x < 0) { //left

        Vector2 leftTop = {nBP.x, bP.y};
        Vector2 left = {nBP.x, bP.y + 1.0f};
        Vector2 leftDown = {nBP.x, bP.y + 1.99f};

        if (isCollision({nBP.x, bP.y}) || isCollision({nBP.x, bP.y + 1.0f}) ||
            isCollision({nBP.x, bP.y + 1.99f})) {
            nBP.x = (int) nBP.x + 1;
            player.m_velocity.x = -0.5*player.m_velocity.x;
            player.m_hitWall = true;
            }
    }
    if (movementDirection.x > 0) { //right

        Vector2 rightTop = {nBP.x + 2.0f, bP.y};
        Vector2 right = {nBP.x + 2.0f, bP.y + 1.0f};
        Vector2 rightDown = {nBP.x + 2.0f, bP.y + 1.99f};

        if (isCollision({nBP.x + 2.0f, bP.y}) || isCollision({nBP.x + 2.0f, bP.y + 1.0f}) ||
            isCollision({nBP.x + 2.0f, bP.y + 1.99f})) {
            nBP.x = (int) nBP.x;
            player.m_velocity.x = -0.5*player.m_velocity.x;
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
            nBP.y = (int) nBP.y + 1;
            player.m_velocity.y = -0.5*player.m_velocity.y;
            player.m_hitWall = true;
        }
    }
    else { //down

        Vector2 downLeft = {nBP.x, nBP.y + 2.0f};
        Vector2 downMid = {nBP.x + 1.0f, nBP.y + 2.0f};
        Vector2 downRight = {nBP.x + 1.90f, nBP.y + 2.0f};

        if (isCollision({nBP.x, nBP.y + 2.0f}) || isCollision({nBP.x + 1.0f, nBP.y + 2.0f}) ||
            isCollision({nBP.x + 1.90f, nBP.y + 2.0f})) {
            nBP.y = (int) nBP.y;
            player.m_velocity.y = 0;
            player.onGround = true;
        }
    }
    player.m_position = nBP;
}

void LevelMap::resolveCollision(Player &player, CollisionSide collisionSide)
{
    switch (collisionSide) {
        case CollisionSide::LEFT: 
            break;
        case CollisionSide::RIGHT: 
            break;
        case CollisionSide::TOP: 
            break;
        case CollisionSide::DOWN: 
            break;
       
    }
}

// void helper(std::vector<Edge>& edges, std::vector<int>& tileMap, int tileSize, int widthCount, int heightCount, bool iterateX, int constValue) {

//     for(int i=0; i<(iterateX ? widthCount : heightCount); i++) {
//         int x = (iterateX ? i : constValue);
//         int y = (iterateX ? constValue : i);
//         int index = y*widthCount + x;
//         if (tileMap.at(index) == 0) {
//             Edge edge;
//             edge.assignedNextLevel = -1;
//             edge.position = {x*tileSize, y*tileSize};

//             if (edges.empty())
//                 edges.push_back(edge);
//             else if ((iterateX ? edges.back().position.y : edges.back().position.x) == constValue) 
//                 (iterateX ? edges.back().position.x : edges.back().position.y) = i*tileSize;
//         }
//     }
// }

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
                background.draw({static_cast<float>(xPos),static_cast<float>(yPos)}, {static_cast<float>(tileSize), static_cast<float>(tileSize)}, {(float) widthCount*tileSize, (float) heightCount*tileSize});
        }
    }

    for(int i=0; i<heightCount; i++) {
        for(int j=0; j<widthCount; j++) {
            int tileIndex = i*widthCount+ j;
            int xPos = j*tileSize;
            int yPos = i*tileSize;
            Color tileColor = WHITE;

            // if (i%background.xCount == 0 && j%background.yCount == 0)
            //     background.draw({static_cast<float>(xPos),static_cast<float>(yPos)}, {static_cast<float>(tileSize), static_cast<float>(tileSize)}, {(float) widthCount*tileSize, (float) heightCount*tileSize});

            // int tile = levelRepository.getLevel(currentTileMapIndex).get(tileIndex);
            // if (tile == 1) {
            //     tileColor = BLUE;
            //     grass.draw({static_cast<float>(xPos),static_cast<float>(yPos)});
            // }
            // if (tile == 3) {tileColor= ORANGE;}

            int tile = level.get(tileIndex);
            CellType cellType = level.getTileCollision(tileIndex);
            if (tile != -1 ) {
                if (debugInfo || cellType != CellType::Spawn)
                    assets.getSprite(tile).draw({static_cast<float>(xPos),static_cast<float>(yPos)}, {(float) tileSize, (float) tileSize});
            }
            // DrawRectangle(xPos, yPos, ileSize, ileSize, tileColor);

            if (!debugInfo) continue;

            char msg1[64];
            snprintf(msg1, sizeof(msg1), "%i, %i", j, i);
            char msg2[64];
            snprintf(msg2, sizeof(msg2), "%i, %i", xPos, yPos);
            if (cellType == CellType::Solid ) {
                DrawLineEx({(float) xPos, (float) yPos}, {(float) (xPos + tileSize),  (float) yPos}, 1, BLACK);
                DrawLineEx({(float) (xPos + tileSize), (float) yPos}, {(float) (xPos + tileSize), (float) (yPos + tileSize)}, 1, BLACK);
                DrawText(msg1, xPos, yPos, 5, BLACK);
            }
            // DrawLineEx((Vector2) {xPos, yPos}, (Vector2) {xPos + tileSize, yPos}, 1, GRAY);
            // DrawLineEx((Vector2) {xPos, yPos}, (Vector2) {xPos, yPos + tileSize}, 1, GRAY);
            // DrawLineEx((Vector2) {xPos+tileSize, yPos}, (Vector2) {xPos + tileSize, yPos + tileSize}, 1, GRAY);
            // DrawLineEx((Vector2) {xPos, yPos+tileSize}, (Vector2) {xPos + tileSize, yPos+ tileSize}, 1, GRAY);
            // DrawText(msg1, xPos, yPos, 5, BLACK);
            // DrawText(msg2, xPos, yPos+10, 5, BLACK);
        }
    }
}