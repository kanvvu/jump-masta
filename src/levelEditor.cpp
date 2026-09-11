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
    createTileNames();
    buildEdges();
}

LevelEditor::LevelEditor(TileLevelManager& tileLevelManager) : LevelEditor(tileLevelManager, 64)
{

}

std::vector<Edge> LevelEditor::getEdges()
{
    std::vector<Edge> edges;


    std::vector<int>& tileMap = currMap;
    int widthCount = tileCountX;
    int heightCount = tileCountY;

    for(int i=0; i<widthCount; i++) {
        //top
        int index = i;
        if (tileMap.at(index) == 0) {
            Edge edge;
            edge.type = TOP;
            edge.assignedNextLevel = -1;
            edge.tilesCount = 1;
            edge.start = {i, 0};
            edge.end = {(i+1), 0};

            if (i != 0 && !edges.empty() && edges.back().end.y == 0 && edges.back().end.x == edge.start.x) { 
                edges.back().end.x = (i+1);
                edges.back().tilesCount++;
            }
            else
                edges.push_back(edge);
        }
    }

    for(int i=0; i<widthCount; i++) {
        //bottom
        int index = (heightCount-1)*widthCount + i;
        if (tileMap.at(index) == 0) {
            Edge edge;
            edge.type = DOWN;
            edge.assignedNextLevel = -1;
            edge.tilesCount = 1;
            edge.start = {i, (heightCount)};
            edge.end = {(i+1), (heightCount)};

            if (i != 0 && !edges.empty() && edges.back().end.y == (heightCount) && edges.back().end.x == edge.start.x) {
                edges.back().end.x = (i+1);
                edges.back().tilesCount++;
            }
            else 
                edges.push_back(edge);
        }
    }

    for(int j=0; j<heightCount; j++) {
        // left
        int index = j*widthCount;
        if (tileMap.at(index) == 0) {
            Edge edge;
            edge.type = LEFT;
            edge.assignedNextLevel = -1;
            edge.tilesCount = 1;
            edge.start = {0, j};
            edge.end = {0, (j+1)};

            if (j != 0 && !edges.empty() && edges.back().end.x == 0 && edges.back().end.y == edge.start.y) {
                edges.back().end.y = (j+1);
                edges.back().tilesCount++;
            }
            else 
                edges.push_back(edge);
        }
    }


    for(int j=0; j<heightCount; j++) {
        // right
        int index = j*widthCount + (widthCount-1);
        if (tileMap.at(index) == 0) {
            Edge edge;
            edge.type = RIGHT;
            edge.assignedNextLevel = -1;
            edge.tilesCount = 1;
            edge.start = {widthCount, j};
            edge.end = {widthCount, (j+1)};

            if (j != 0 && !edges.empty() && edges.back().end.x == widthCount && edges.back().end.y == edge.start.y) {
                edges.back().end.y = (j+1);
                edges.back().tilesCount++;
            }
            else 
                edges.push_back(edge);
        }
    }

    return edges;
}

void LevelEditor::buildEdges()
{
    edges = getEdges();
    gui.rebuildDropDowns();

    gui.dropDowns.clear();
    for (int i=0; i<edges.size(); i++) {
        gui.dropDowns.push_back(DropDownState());
        gui.dropDowns.back().dropdownActive = edges.at(i).assignedNextLevel;
    }

}

void LevelEditor::saveLevel(std::string name) {
    TileMapLevel tileMapLevel;
    tileMapLevel.width = tileCountX;
    tileMapLevel.height = tileCountY;
    tileMapLevel.level = currMap;
    tileMapLevel.name = name + std::to_string(tileLevelManager.getLevelCount());

    tileLevelManager.saveLevel(tileMapLevel, edges);

    createTileNames();
}

void LevelEditor::loadLevel(int levelID) {
    TileMapLevel tileMapLevel = tileLevelManager.getLevel(levelID);

    tileCountX = tileMapLevel.width;
    tileCountY = tileMapLevel.height;

    currMap.clear();
    for(int i : tileMapLevel.level) {
        currMap.push_back(i);
    }

    buildEdges();

    std::vector<Edge> assigned = tileLevelManager.getAssigned(levelID);
    if (assigned.size() != edges.size()) {
        std::cout<<"\033[31m"<<"ERROR assigned not equal edges!\033[0m\n";
    } else {
        for(int i=0; i<edges.size(); i++) {
            edges[i].assignedNextLevel = assigned[i].assignedNextLevel;
        }
    }

    gui.dropDowns.clear();
    for (int i=0; i<edges.size(); i++) {
        gui.dropDowns.push_back(DropDownState());
        gui.dropDowns.back().dropdownActive = edges.at(i).assignedNextLevel;
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
        saveLevel("level");
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

            buildEdges();
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

    
    drawEdges();
    
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
        tileListSelected = (tileList.empty() ? -1 : tileList.size()-1);
    }
}

void LevelEditor::EditorGuiState::rebuildDropDowns()
{
    dropDowns.clear();
    dropDownOptions = "";
    for(int i=0; i<tileNames.size(); i++) {
        dropDownOptions += tileNames.at(i);
        if (i + 1 != tileNames.size()) dropDownOptions += ";";
    }
}

void LevelEditor::drawGUI()
{
    if (GuiButton({ 10, 100, 220, 60 }, "Add")) {
        saveLevel("level");
        // std::cout<<"SAVE!"<<std::endl;
    }
    if (GuiButton({ 10 + 10 + 220 , 100, 220, 60 }, "Save All")) {
        // saveLevel("level");
        tileLevelManager.saveLevels();
        // std::cout<<"SAVE!"<<std::endl;
    }

    GuiListViewEx({ 10, 300, 300, 200 }, gui.tileList.data(), static_cast<int>(gui.tileList.size()), &gui.tileListScroll, &gui.tileListSelected, &gui.tileListHovered);

    if (GuiButton({ 10, 300 + 210, 220, 60 }, "Load")){
        if (gui.tileListSelected >=0 && gui.tileListSelected < tileLevelManager.getLevelCount()) {
            loadLevel(gui.tileListSelected);
        } else {
            std::cout<<"ERROR LOAD! "<<gui.tileListSelected<<std::endl;
        }
        // std::cout<<"LOAD!"<<gui.tileListSelected<<std::endl;
    }

    if (GuiButton({ 10 + 10 + 220, 300 + 210, 220, 60 }, "Delete")){
        bool isOk = true;
        if (gui.tileListSelected >=0 && gui.tileListSelected < tileLevelManager.getLevelCount()) {
            tileLevelManager.deleteLevel(gui.tileListSelected);
        } else {
            std::cout<<"ERROR DELETE!"<<gui.tileListSelected<<std::endl;
            isOk = false;
        }

        currMap = std::vector<int>(tileCountX*tileCountY, 0);
        createTileNames();
        buildEdges();
    }
    
    if (GuiButton({ 10, 300 + 210 + 10 + 60, 220, 60 }, "Save")){
        if (gui.tileListSelected >=0 && gui.tileListSelected < tileLevelManager.getLevelCount()) {
            TileMapLevel level = tileLevelManager.getLevel(gui.tileListSelected);
            level.level = currMap;
            tileLevelManager.editLevel(gui.tileListSelected, level, edges);
        } else {
            std::cout<<"ERROR LOAD! "<<gui.tileListSelected<<std::endl;
        }

    }

    for(int i=edges.size(); i>0; i--) {
        Edge& e = edges.at(i-1);
        std::string str = "EDGE " +std::to_string(i) + " (" + std::to_string(e.tilesCount) + ")"; 
        Vector2 textPos = {10, (i-1)*30 + 300 + 210 + 10 + 60 + 80};
        DrawText(str.c_str(), textPos.x, textPos.y, 20, BLACK);
        int textWidth = MeasureText(str.c_str(), 20);
        if (GuiDropdownBox({ textPos.x + textWidth + 10, textPos.y, 200, 25 }, gui.dropDownOptions.c_str(), &gui.dropDowns[i-1].dropdownActive, gui.dropDowns[i-1].dropDownEditMode)) {
                gui.dropDowns[i-1].dropDownEditMode = !gui.dropDowns[i-1].dropDownEditMode; // Toggle open/closed state
               
                e.assignedNextLevel = gui.dropDowns[i-1].dropdownActive;
        }
    }
}

void LevelEditor::drawEdges()
{
    
    // std::cout<<"EDGES: "<<edges.size()<<std::endl;
    int i=1;
    for(Edge& e : edges) {
        Color color = RED;
        if (e.assignedNextLevel != -1) color = GREEN;
        DrawLineEx({(float)e.start.x*tileSize,(float) e.start.y*tileSize}, {(float)e.end.x*tileSize,(float) e.end.y*tileSize},4, color);
        std::string str = "EDGE " +std::to_string(i) + " (" + std::to_string(e.tilesCount) + ")"; 
        Vector2 textPos = {e.start.x*tileSize, e.start.y*tileSize};
        int textWidth = MeasureText(str.c_str(), 20);
        if (e.type == TOP) {
            textPos.y -= 20; 
        } else if (e.type == LEFT) {
            textPos.x -= textWidth - 1;
        }
        DrawText(str.c_str(), textPos.x, textPos.y, 20, BLACK);
        i++;
    }
}

void LevelEditor::createTileNames()
{
    gui.tileNames.clear();

    int levelCount = tileLevelManager.getLevelCount();
    for (int i=0; i<levelCount; i++) {
        gui.tileNames.push_back(std::to_string(i+1) + ". " + tileLevelManager.getLevel(i).name);
    }

    gui.rebuildTileList();
}
