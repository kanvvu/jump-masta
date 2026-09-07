#include "levelEditor.hpp"

#include "raylib.h"
#include "raymath.h"
#include <iostream>
#include <string>

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

LevelEditor::LevelEditor(TileLevelManager& tileLevelManager, int tileSize) : m_tileSize(tileSize), tileLevelManager(tileLevelManager)
{
    camera.offset.x = screenWidth/2 - (tileCountX*tileSize)/2;   // coordinate x origin
    camera.offset.y = screenHeight/2 - (tileCountY*tileSize)/2;   // coordinate y origin
    camera.target.x = 0;         // rotation and zoom x origin (from offset above)
    camera.target.y = 0;         // rotation and zoom y origin (from offset above)
    camera.rotation = 0;        // rotation in deg
    camera.zoom     = 1.0f;      // magnification, i.e fov or zoom

    currMap = std::vector<int>(tileCountX*tileCountY, 0);
    temp();
}

LevelEditor::LevelEditor(TileLevelManager& tileLevelManager) : LevelEditor(tileLevelManager, 64)
{

}

void saveLevel(TileLevelManager& tileLevelManager, int width, int height, const std::vector<int>& level, std::string name) {
    TileMapLevel tileMapLevel;
    tileMapLevel.width = width;
    tileMapLevel.height = height;
    tileMapLevel.level = level;
    tileMapLevel.name = name + std::to_string(tileLevelManager.getLevelCount());

    tileLevelManager.saveLevel(tileMapLevel);
}

void loadLevel(TileLevelManager& tileLevelManager,int levelID, int& width, int& height, std::vector<int>& level) {
    TileMapLevel tileMapLevel = tileLevelManager.getLevel(levelID);

    width = tileMapLevel.width;
    height = tileMapLevel.height;

    level.clear();
    for(int i : tileMapLevel.level) {
        level.push_back(i);
    }
}

void LevelEditor::proccessInput(GameState &gameState)
{
    if (IsKeyPressed(KEY_E)) {
        gameState = GAME;
    }   

    if (IsKeyPressed(KEY_R)) {
        for (int& tile : currMap) tile = 0;
    }

    if (IsKeyPressed(KEY_S)) {
        saveLevel(tileLevelManager, tileCountX, tileCountY, currMap, "level");
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
            // std::cout<<"TILE INDEX: "<<tileIndex <<"\n";
            // std::cout<<"firstTime: "<<firstTime<<", "<<"mouseMode: "<<mouseMode <<"\n";

            if (firstTime) {
                if (currMap.at(tileIndex) == selectedTile) mouseMode = 1;
                else mouseMode = 0;
            }
            
            if (mouseMode == 0)
                currMap.at(tileIndex) = 1; 
            else
                currMap.at(tileIndex) = 0; 
        } 
        // std::cout<<xPos<<", "<<yPos<<"\n";
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

    GuiSetStyle(DEFAULT, TEXT_SIZE, GuiGetFont().baseSize*3);
    BeginMode2D(camera);

    for(int i=0; i<tileCountY; i++) {
        for(int j=0; j<tileCountX; j++) {
            int tileIndex = i*tileCountX + j;
            int xPos = j*m_tileSize;
            int yPos = i*m_tileSize;
            Color tileColor = WHITE;

            // std::vector<int>& tileMap = tileMaps[currentTileMapIndex];
            if (currMap.at(tileIndex) == 1) tileColor = BLUE;
            if (currMap[tileIndex] == 3) tileColor= ORANGE;


            DrawRectangle(xPos, yPos, m_tileSize, m_tileSize, tileColor);
        }
    }

    
    Vector2 mousePos = GetMousePosition();
    if (mouseMode == 0 && mousePos.x - camera.offset.x >= 0 && mousePos.x - camera.offset.x < tileCountX*tileSize && mousePos.y - camera.offset.y >= 0 && mousePos.y - camera.offset.y < tileCountY*tileSize) 
        DrawRectangle(floor((mousePos.x-camera.offset.x)/tileSize)*tileSize, floor((mousePos.y-camera.offset.y)/tileSize)*tileSize, tileSize, tileSize, LIGHTGRAY);
    // std::cout<<"("<<mousePos.x <<", "<<mousePos.y<<"), ";
    // std::cout<<"("<<camera.offset.x <<", "<<camera.offset.y<<"), ";
    // std::cout<<mousePos.x - camera.offset.x <<", "<<mousePos.y - camera.offset.y<<"\n";

    
    for(int j=0; j<tileCountX+1; j++) {
        Vector2 start = {static_cast<float>(j*64), 0.0f};
        Vector2 end = {static_cast<float>(j*64), tileCountY*64.0f};
        DrawLineEx(start, end, 1, BLACK);
    }

    for(int i=0; i<tileCountY+1; i++) {
        Vector2 start = {0.0f, static_cast<float>(i*64)};
        Vector2 end = {tileCountX*64.0f, static_cast<float>(i*64)};
        DrawLineEx(start, end, 1, BLACK);
    }

    
    
    EndMode2D();

    drawGUI();
}

void LevelEditor::EditorGuiState::rebuildTileList()
{
    tileList.clear();

    for (std::string& tileName : tileNames) {
        tileList.push_back(const_cast<char*>(tileName.c_str()));
    }
    
    if (tileListSelected >= static_cast<int>(tileList.size())) {
        tileListSelected = tileList.empty() ? -1 : 0;
    }
}



void LevelEditor::drawGUI()
{
    if (GuiButton({ 10, 100, 220, 60 }, "Save")) {
        saveLevel(tileLevelManager, tileCountX, tileCountY, currMap, "level");
        temp();
        // std::cout<<"SAVE!"<<std::endl;
    }

    GuiListViewEx({ 10, 300, 300, 200 }, gui.tileList.data(), static_cast<int>(gui.tileList.size()), &gui.tileListScroll, &gui.tileListSelected, &gui.tileListHovered);

    if (GuiButton({ 10, 300 + 210, 220, 60 }, "Load")){
        if (gui.tileListSelected >=0 && gui.tileListSelected < tileLevelManager.getLevelCount()) {
            loadLevel(tileLevelManager, gui.tileListSelected, tileCountX, tileCountY, currMap);
        } else {
            std::cout<<"ERROR LOAD!"<<gui.tileListSelected<<std::endl;
        }
        // std::cout<<"LOAD!"<<gui.tileListSelected<<std::endl;
    }
}

void LevelEditor::temp()
{
    gui.tileNames.clear();

    int levelCount = tileLevelManager.getLevelCount();
    for (int i=0; i<levelCount; i++) {
        gui.tileNames.push_back(std::to_string(i+1) + ". " + tileLevelManager.getLevel(i).name);
    }

    gui.rebuildTileList();
}
