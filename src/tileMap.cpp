#include "tileMap.hpp"
#include <iostream>
#include "raylib.h"

TileMap::TileMap(TileLevelManager& tileLevelManager, int widthCount, int heightCount)
: tileLevelManager(tileLevelManager),
  m_widthCount(widthCount),
  m_heightCount(heightCount)
{

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

