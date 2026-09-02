#include "levelEditor.hpp"

#include "raylib.h"
#include "raymath.h"
#include <iostream>

LevelEditor::LevelEditor(int tileSize) : m_tileSize(tileSize) 
{
    camera.offset.x = 1920/2 - (10*64)/2;   // coordinate x origin
    camera.offset.y = 1080/2 - (15*64)/2;   // coordinate y origin
    camera.target.x = 0;         // rotation and zoom x origin (from offset above)
    camera.target.y = 0;         // rotation and zoom y origin (from offset above)
    camera.rotation = 0;        // rotation in deg
    camera.zoom     = 1.0f;      // magnification, i.e fov or zoom
}

LevelEditor::LevelEditor() : LevelEditor(64)
{

}

void LevelEditor::proccessInput(GameState &gameState)
{
    if (IsKeyPressed(KEY_E)) {
        gameState = GAME;
    }   
}

void LevelEditor::update(float deltaTime)
{
    if (IsMouseButtonDown(1)) {
        camera.offset = Vector2Add(camera.offset, GetMouseDelta());
    }
}

void LevelEditor::draw()
{
    ClearBackground(LIGHTGRAY);

    BeginMode2D(camera);
    
    Vector2 mousePos = GetMousePosition();
    DrawRectangle(floor((mousePos.x-camera.offset.x)/64)*64, floor((mousePos.y-camera.offset.y)/64)*64, 64, 64, WHITE);
    std::cout<<"("<<mousePos.x <<", "<<mousePos.y<<"), ";
    std::cout<<"("<<camera.offset.x <<", "<<camera.offset.y<<"), ";
    std::cout<<mousePos.x - camera.offset.x <<", "<<mousePos.y - camera.offset.y<<"\n";

    
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