#include "levelMap.hpp"
#include <iostream>
#include "raylib.h"
#include "raymath.h"


LevelMap::LevelMap(LevelRepository& levelRepository, Sprite background, Sprite grass) 
    : levelRepository(levelRepository),
    background(background),
    grass(grass)
{

}

void LevelMap::changeLevel(int levelID)
{
    if (levelID >= levelRepository.getLevelCount()) {
        std::cout<<"ERROR [TileMap::changeLevel] levelID is "<<levelID<<" where max: "<<levelRepository.getLevelCount()<<"\n";
        return;
    }

    currentTileMapIndex = levelID;
}


void LevelMap::draw()
{
    int m_widthCount = levelRepository.getLevel(currentTileMapIndex).getWidth();
    int m_heightCount = levelRepository.getLevel(currentTileMapIndex).getHeight();
    int m_tileSize = getTileSize();

    for(int i=0; i<m_heightCount; i++) {
        for(int j=0; j<m_widthCount; j++) {
            int tileIndex = i*m_widthCount+ j;
            int xPos = j*m_tileSize;
            int yPos = i*m_tileSize;
            Color tileColor = WHITE;

            background.draw({static_cast<float>(xPos),static_cast<float>(yPos)});

            int tile = levelRepository.getLevel(currentTileMapIndex).get(tileIndex);
            if (tile == 1) {
                tileColor = BLUE;
                grass.draw({static_cast<float>(xPos),static_cast<float>(yPos)});
            }
            if (tile == 3) {tileColor= ORANGE;}

            // DrawRectangle(xPos, yPos, m_tileSize, m_tileSize, tileColor);
            char msg1[64];
            snprintf(msg1, sizeof(msg1), "%i, %i", j, i);
            char msg2[64];
            snprintf(msg2, sizeof(msg2), "%i, %i", xPos, yPos);
            if (tile == 1) {
                DrawLineEx({(float) xPos, (float) yPos}, {(float) (xPos + m_tileSize),  (float) yPos}, 1, BLACK);
                DrawLineEx({(float) (xPos + m_tileSize), (float) yPos}, {(float) (xPos + m_tileSize), (float) (yPos + m_tileSize)}, 1, BLACK);
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

int LevelMap::getTileSize()
{
    return levelRepository.getLevel(currentTileMapIndex).getTileSize();
}

bool LevelMap::isCollision(Vector2 position, int tileType) const
{
    int m_widthCount = levelRepository.getLevel(currentTileMapIndex).getWidth();
    int m_heightCount = levelRepository.getLevel(currentTileMapIndex).getHeight();

    int posX = position.x;
    int posY = position.y;
    if (posX >= m_widthCount|| posX < 0 || posY >= m_heightCount || posY < 0) {
        // printf("COLLSION POSITION OUT OF BOUNDS!\n");
        return false;
    }

    const LevelData& level = levelRepository.getLevel(currentTileMapIndex);
    if (level.get(posY*m_widthCount + posX) == tileType) return true;
    
    return false;
}

Vector2 LevelMap::getSpawnPosition()
{
    int m_widthCount = levelRepository.getLevel(currentTileMapIndex).getWidth();
    int m_heightCount = levelRepository.getLevel(currentTileMapIndex).getHeight();
    int m_tileSize = getTileSize();

    for(int i=0; i<m_heightCount; i++) {
        for(int j=0; j<m_widthCount; j++) {
            int tileIndex = i*m_widthCount+ j;
            int xPos = j*m_tileSize;
            int yPos = i*m_tileSize;
            Color tileColor = WHITE;

            int tile = levelRepository.getLevel(currentTileMapIndex).get(tileIndex);
            if (tile == 2) return {(float) j,(float) i};
        }
    }
    return {0,0};
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
        // std::cout<<playerPosition.y<<" "<<edge.start.y<<std::endl;
        int m_widthCount = levelRepository.getLevel(edge.assignedNextLevel).getWidth();
        int m_heightCount = levelRepository.getLevel(edge.assignedNextLevel).getHeight();

        switch (edge.type)
        {
        case TOP:
            if (playerPosition.y + 0.5f < edge.start.y) {
                changeLevel(edge.assignedNextLevel);
                playerPosition.y = m_heightCount - 1.0f;
                return true;
            }
            break;
        case DOWN:
            if (playerPosition.y + 0.5f> edge.start.y) {
                changeLevel(edge.assignedNextLevel);
                playerPosition.y = 0.0f;
                return true;
            }
            break;
        case LEFT:
            if (playerPosition.x + 0.5f < edge.start.x) {
                changeLevel(edge.assignedNextLevel);
                playerPosition.x = m_widthCount - 1.0f;
                return true;
            }
            break;
        case RIGHT:
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

void LevelMap::checkAndResolveCollision(Player& player) {
    Vector2 bP = player.m_previousPosition; 
    Vector2 nBP = player.m_position;
    
    Vector2 movementDirection = Vector2Subtract(nBP, bP);
    if (movementDirection.x <= 0) { //left
        if (isCollision({nBP.x, bP.y}, 1) ||
            isCollision({nBP.x, bP.y + 0.99f}, 1)) {
            nBP.x = (int) nBP.x + 1;
            player.m_velocity.x = -0.5*player.m_velocity.x;
            }
    }
    else {  //right
        if (isCollision({nBP.x + 1.0f, bP.y}, 1) ||
            isCollision({nBP.x + 1.0f, bP.y + 0.99f}, 1)) {
            nBP.x = (int) nBP.x;
            player.m_velocity.x = -0.5*player.m_velocity.x;
        }
    }

    player.onGround = false;
    if (movementDirection.y <= 0) { //up
        if (isCollision({nBP.x, nBP.y}, 1) ||
            isCollision({nBP.x + 0.90f, nBP.y}, 1)) {
            nBP.y = (int) nBP.y + 1;
            player.m_velocity.y = -0.5*player.m_velocity.y;
        }
    }
    else { //down
        if (isCollision({nBP.x, nBP.y + 1.0f}, 1) ||
            isCollision({nBP.x + 0.90f, nBP.y + 1.0f}, 1)) {
            nBP.y = (int) nBP.y;
            player.m_velocity.y = 0;
            player.onGround = true;
        }
    }
    player.m_position = nBP;
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

