#include "tileMap.hpp"
#include <iostream>
#include "raylib.h"


TileMap::TileMap(TileLevelManager &tileLevelManager, int widthCount, int heightCount)
    : tileLevelManager(tileLevelManager),
      m_widthCount(widthCount),
      m_heightCount(heightCount)
{

}

void TileMap::changeLevel(int levelID)
{
    if (levelID >= tileLevelManager.getLevelCount()) {
        std::cout<<"ERROR [changeLevel] levelID is "<<levelID<<" where max: "<<tileLevelManager.getLevelCount()<<"\n";
        return;
    }

    currentTileMapIndex = levelID;
    m_widthCount = tileLevelManager.getLevel(levelID).width;
    m_heightCount = tileLevelManager.getLevel(levelID).height;
}

void TileMap::draw()
{
    for(int i=0; i<m_heightCount; i++) {
        for(int j=0; j<m_widthCount; j++) {
            int tileIndex = i*m_widthCount+ j;
            int xPos = j*m_tileSize;
            int yPos = i*m_tileSize;
            Color tileColor = WHITE;

            std::vector<int>& tileMap = tileLevelManager.getLevel(currentTileMapIndex).level;
            if (tileMap[tileIndex] == 1) tileColor = BLUE;
            if (tileMap[tileIndex] == 3) tileColor= ORANGE;

            DrawRectangle(xPos, yPos, m_tileSize, m_tileSize, tileColor);
            char msg1[64];
            snprintf(msg1, sizeof(msg1), "%i, %i", j, i);
            char msg2[64];
            snprintf(msg2, sizeof(msg2), "%i, %i", xPos, yPos);
            if (tileMap[tileIndex] == 1) {
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

bool TileMap::isCollision(Vector2 position, int tileType) const
{
    int posX = position.x;
    int posY = position.y;
    if (posX >= m_widthCount|| posX < 0 || posY >= m_heightCount || posY < 0) {
        // printf("COLLSION POSITION OUT OF BOUNDS!\n");
        return false;
    }

    const std::vector<int>& tileMap = tileLevelManager.getLevel(currentTileMapIndex).level;
    if (tileMap[posY*m_widthCount + posX] == tileType) return true;
    
    return false;
}

Vector2 TileMap::getSpawnPosition()
{
    for(int i=0; i<m_heightCount; i++) {
        for(int j=0; j<m_widthCount; j++) {
            int tileIndex = i*m_widthCount+ j;
            int xPos = j*m_tileSize;
            int yPos = i*m_tileSize;
            Color tileColor = WHITE;

            std::vector<int>& tileMap = tileLevelManager.getLevel(currentTileMapIndex).level;
            if (tileMap[tileIndex] == 2) return {(float) j,(float) i};
        }
    }
    return {0,0};
}

void TileMap::checkForNextLevel(Vector2& playerPosition)
{
    if (tileLevelManager.getEdges(currentTileMapIndex).size() == 0) return;

    std::vector<Edge>& assignedEdges = tileLevelManager.getEdges(currentTileMapIndex); 
    std::vector<Edge> edges = tileLevelManager.createEdges(tileLevelManager.getLevel(currentTileMapIndex).level, m_widthCount, m_heightCount);

    if (assignedEdges.size() != edges.size()) {
        std::cerr<<"ERROR [checkForNextLevel] "<<assignedEdges.size()<<" != "<<edges.size()<<std::endl;
        return;
    }

    for(int i=0; i<edges.size(); i++) {
        const Edge& aEdge = assignedEdges.at(i);
        const Edge& edge = edges.at(i);

        if (aEdge.assignedNextLevel == -1) continue;
        // std::cout<<playerPosition.y<<" "<<edge.start.y<<std::endl;
        switch (edge.type)
        {
        case TOP:
            if (playerPosition.y < edge.start.y) {
                changeLevel(aEdge.assignedNextLevel);
                playerPosition.y = m_heightCount -1;
            }
            break;
        case DOWN:
            if (playerPosition.y + 1.0f> edge.start.y) {
                changeLevel(aEdge.assignedNextLevel);
                playerPosition.y = 0.0f;
            }
            break;
        case LEFT:
            if (playerPosition.x < edge.start.x) {
                changeLevel(aEdge.assignedNextLevel);
                playerPosition.x = m_widthCount - 1;
            }
            break;
        case RIGHT:
            if (playerPosition.x + 1.0f > edge.start.x) {
                changeLevel(aEdge.assignedNextLevel);
                playerPosition.x = 0.0f;
            }
            break;
        }
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

