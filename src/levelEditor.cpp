#include "levelEditor.hpp"

#include "raylib.h"
#include "raymath.h"
#include <iostream>

LevelEditor::LevelEditor(int tileSize) : m_tileSize(tileSize) 
{
    camera.offset.x = screenWidth/2 - (tileCountX*tileSize)/2;   // coordinate x origin
    camera.offset.y = screenHeight/2 - (tileCountY*tileSize)/2;   // coordinate y origin
    camera.target.x = 0;         // rotation and zoom x origin (from offset above)
    camera.target.y = 0;         // rotation and zoom y origin (from offset above)
    camera.rotation = 0;        // rotation in deg
    camera.zoom     = 1.0f;      // magnification, i.e fov or zoom

    currMap = std::vector<int>(tileCountX*tileCountY, 0);
}

LevelEditor::LevelEditor() : LevelEditor(64)
{

}

void LevelEditor::proccessInput(GameState &gameState)
{
    if (IsKeyPressed(KEY_E)) {
        gameState = GAME;
    }   

    if (IsKeyPressed(KEY_R)) {
        for (int& tile : currMap) tile = 0;
    }

    if (IsMouseButtonDown(1)) {
        camera.offset = Vector2Add(camera.offset, GetMouseDelta());
    }

    bool firstTime = false;
    if (IsMouseButtonDown(0) && isMouseDown == false) {
        isMouseDown = true;
        firstTime = true;
    }

    if (!IsMouseButtonDown(0)) isMouseDown = false;


    if (IsMouseButtonDown(0)) {
        Vector2 mousePos = GetMousePosition();
        int xPos = (mousePos.x - camera.offset.x)/tileSize;
        int yPos = (mousePos.y - camera.offset.y)/tileSize;

        if (xPos >= 0 && xPos < tileCountX && yPos >=0 && yPos < tileCountY) {
            int tileIndex = yPos*tileCountX + xPos;
            std::cout<<"TILE INDEX: "<<tileIndex <<"\n";
            std::cout<<"firstTime: "<<firstTime<<", "<<"mouseMode: "<<mouseMode <<"\n";

            if (firstTime) {
                if (currMap.at(tileIndex) == selectedTile) mouseMode = 1;
                else mouseMode = 0;
            }
            if (mouseMode == 0)
                currMap.at(tileIndex) = 1; 
            else
                currMap.at(tileIndex) = 0; 
        } 
        std::cout<<xPos<<", "<<yPos<<"\n";
    }
}

void LevelEditor::update(float deltaTime)
{
    // Vector2 mousePos = GetMousePosition();
    // std::cout<<mousePos.x - camera.offset.x <<", "<<mousePos.y - camera.offset.y<<"\n";
}

void LevelEditor::draw()
{
    ClearBackground(LIGHTGRAY);

    BeginMode2D(camera);

    for(int i=0; i<tileCountY; i++) {
        for(int j=0; j<tileCountX; j++) {
            int tileIndex = i*tileCountX + j;
            int xPos = j*m_tileSize;
            int yPos = i*m_tileSize;
            Color tileColor = LIGHTGRAY;

            // std::vector<int>& tileMap = tileMaps[currentTileMapIndex];
            if (currMap.at(tileIndex) == 1) tileColor = WHITE;
            // if (currMap[tileIndex] == 0) tileColor= ORANGE;

            DrawRectangle(xPos, yPos, m_tileSize, m_tileSize, tileColor);
        }
    }

    
    Vector2 mousePos = GetMousePosition();
    DrawRectangle(floor((mousePos.x-camera.offset.x)/tileSize)*tileSize, floor((mousePos.y-camera.offset.y)/tileSize)*tileSize, tileSize, tileSize, WHITE);
    // std::cout<<"("<<mousePos.x <<", "<<mousePos.y<<"), ";
    // std::cout<<"("<<camera.offset.x <<", "<<camera.offset.y<<"), ";
    // std::cout<<mousePos.x - camera.offset.x <<", "<<mousePos.y - camera.offset.y<<"\n";

    
    for(int j=0; j<10+1; j++) {
        Vector2 start = {j*64,0};
        Vector2 end = {j*64,15*64};
        DrawLineEx(start, end, 1, BLACK);
    }

    for(int i=0; i<15+1; i++) {
        Vector2 start = {0,i*64};
        Vector2 end = {10*64,i*64};
        DrawLineEx(start, end, 1, BLACK);
    }


    EndMode2D();
}