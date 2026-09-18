#include "levelEditor.hpp"

#include "raylib.h"
#include "raymath.h"
#include <iostream>
#include <string>
#include <cstring>

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

LevelEditor::LevelEditor(LevelRepository& levelRepository, int tileSize) : m_tileSize(tileSize), levelRepository(levelRepository), currLevel(10, 15)
{
    camera.offset.x = screenWidth/2 - (currLevel.getWidth()*tileSize)/2;   // coordinate x origin
    camera.offset.y = screenHeight/2 - (currLevel.getHeight()*tileSize)/2;   // coordinate y origin
    camera.target.x = 0;         // rotation and zoom x origin (from offset above)
    camera.target.y = 0;         // rotation and zoom y origin (from offset above)
    camera.rotation = 0;        // rotation in deg
    camera.zoom     = 1.0f;      // magnification, i.e fov or zoom

    snprintf(gui.tbWidth, sizeof(gui.tbWidth), std::to_string(currLevel.getWidth()).c_str());
    snprintf(gui.tbHeight, sizeof(gui.tbHeight), std::to_string(currLevel.getHeight()).c_str());
    snprintf(gui.tbName, sizeof(gui.tbName), currLevel.getName().c_str());

    createTileNames();
    buildEdges();
}

LevelEditor::LevelEditor(LevelRepository& tileLevelManager) : LevelEditor(tileLevelManager, 64)
{

}

void LevelEditor::buildEdges()
{
    // currLevel.edges = levelRepository.createEdges(currLevel);

    gui.rebuildDropDowns();

    gui.dropDowns.clear();
    for (int i=0; i<currLevel.getEdgesSize(); i++) {
        gui.dropDowns.push_back(DropDownState());
        gui.dropDowns.back().dropdownActive = currLevel.getEdge(i).assignedNextLevel;
    }

}

void LevelEditor::saveLevel(std::string name) {
    currLevel.setName(name + std::to_string(levelRepository.getLevelCount()));

    levelRepository.saveLevel(currLevel);

    createTileNames();
}

void LevelEditor::loadLevel(int levelID) {
    LevelData levelData = levelRepository.getLevel(levelID);

    snprintf(gui.tbWidth, sizeof(gui.tbWidth), std::to_string(levelData.getWidth()).c_str());
    snprintf(gui.tbHeight, sizeof(gui.tbHeight), std::to_string(levelData.getHeight()).c_str());
    snprintf(gui.tbName, sizeof(gui.tbName), levelData.getName().c_str());

    camera.offset.x = screenWidth/2 - (levelData.getWidth()*tileSize)/2;   // coordinate x origin
    camera.offset.y = screenHeight/2 - (levelData.getHeight()*tileSize)/2;   // coordinate y origin


    currLevel = levelData;
    std::cout<<"LEVELS: "<<currLevel.getLevelSize()<<", EDGES: "<<currLevel.getEdgesSize()<<std::endl;

    buildEdges();

    // std::vector<Edge> assigned = levelData.edges; 
    // if (assigned.size() != currLevel.edges.size()) {
    //     std::cerr<<"\033[31m"<<"ERROR [LevelEditor::loadLevel] assigned not equal edges!\033[0m\n";
    // } else {
    //     for(int i=0; i<currLevel.edges.size(); i++) {
    //         currLevel.edges[i].assignedNextLevel = assigned[i].assignedNextLevel;
    //     }
    // }

    gui.dropDowns.clear();
    for (int i=0; i<currLevel.getEdgesSize(); i++) {
        gui.dropDowns.push_back(DropDownState());
        gui.dropDowns.back().dropdownActive = currLevel.getEdge(i).assignedNextLevel;
    }
}

void LevelEditor::proccessInput(GameState &gameState)
{
    if (!gui.tbNameEditMode) {
        if (IsKeyPressed(KEY_E)) {
            gameState = GAME;
        }   

        if (IsKeyPressed(KEY_R)) {
            // for (int& tile : currLevel.level) tile = 0;
            for(int i=0; i<currLevel.getLevelSize(); i++) {currLevel.set(i, 0);};
        }

        if (IsKeyPressed(KEY_S)) {
            saveLevel("level");
        }
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

        if (xPos >= 0 && xPos < currLevel.getWidth() && yPos >=0 && yPos < currLevel.getHeight()) {
            int tileIndex = yPos*currLevel.getWidth() + xPos;
            // std::cout<<"TILE INDEX: "<<tileIndex <<"\n";
            // std::cout<<"firstTime: "<<firstTime<<", "<<"mouseMode: "<<mouseMode <<"\n";

            if (firstTime) {
                if (currLevel.get(tileIndex) == selectedTile) mouseMode = 1;
                else mouseMode = 0;
            }
            
            if (mouseMode == 0)
                currLevel.set(tileIndex,1);
            else
                currLevel.set(tileIndex, 0);

            // currLevel.createEdges();
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


    currLevel.draw();

    
    Vector2 mousePos = GetMousePosition();
    if (mouseMode == 0 && mousePos.x - camera.offset.x >= 0 && mousePos.x - camera.offset.x < currLevel.getWidth()*tileSize && mousePos.y - camera.offset.y >= 0 && mousePos.y - camera.offset.y < currLevel.getHeight()*tileSize) 
        DrawRectangle(floor((mousePos.x-camera.offset.x)/tileSize)*tileSize, floor((mousePos.y-camera.offset.y)/tileSize)*tileSize, tileSize, tileSize, LIGHTGRAY);
    // std::cout<<"("<<mousePos.x <<", "<<mousePos.y<<"), ";
    // std::cout<<"("<<camera.offset.x <<", "<<camera.offset.y<<"), ";
    // std::cout<<mousePos.x - camera.offset.x <<", "<<mousePos.y - camera.offset.y<<"\n";

    
    for(int j=0; j<currLevel.getWidth()+1; j++) {
        Vector2 start = {static_cast<float>(j*64), 0.0f};
        Vector2 end = {static_cast<float>(j*64), currLevel.getHeight()*64.0f};
        DrawLineEx(start, end, 1, BLACK);
    }

    for(int i=0; i<currLevel.getHeight()+1; i++) {
        Vector2 start = {0.0f, static_cast<float>(i*64)};
        Vector2 end = {currLevel.getWidth()*64.0f, static_cast<float>(i*64)};
        DrawLineEx(start, end, 1, BLACK);
    }

    
    drawEdges();
    
    EndMode2D();

    drawGUI();
}

void LevelEditor::EditorGuiState::changeSelected(int index, int maxScroll)
{
    if (index >= tileNames.size()) {
        std::cerr<<"ERROR [LevelEditor::changeSelected] invalid index: "<<index<<" >="<<tileNames.size()<<"\n";
        return;
    }
    if (index <  0) {
        std::cerr<<"ERROR [LevelEditor::changeSelected] invalid index: "<<index<<" < "<<" 0\n";
        return;
    }

    int scroll = tileList.size() - maxScroll;
    scroll = scroll < 0 ? 0 : scroll;

    if (tileListScroll > scroll) {
        tileListScroll = scroll;
    }

    tileListSelected = index;
    // std::cout<<index<<" "<<scroll<<std::endl;
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


bool isNumber(const char* str, int size) {
    for(int i=0; i<size && str[i] != '\0'; i++) {
        if (!isdigit(str[i])) return false;
    }
    return true;

}

void LevelEditor::drawGUI()
{
    if (GuiTextBox({ 10, 50, 260, 50 }, gui.tbName, 64, gui.tbNameEditMode)) {
        if (gui.tbNameEditMode) {
            // std::cout<<"ENTER\n";
            currLevel.setName(gui.tbName); 
        } 

        gui.tbNameEditMode = !gui.tbNameEditMode;
    }

    if (GuiTextBox({ 10, 50 + 30 + 10 + 20, 125, 50 }, gui.tbWidth, 64, gui.tbWidthEditMode)) {
        if (gui.tbWidthEditMode) {
            if (isNumber(gui.tbWidth, sizeof(gui.tbWidth))) {
                // currLevel.setWidth(std::stoi(gui.tbWidth));
                currLevel.changeLevelDimensions(std::stoi(gui.tbWidth), currLevel.getHeight());
                // levelRepository.changeLevelDimensions(gui.tileListSelected, currLevel.getWidth(), currLevel.getHeight());
                // levelRepository.getLevel(gui.tileListSelected).changeLevelDimensions(currLevel.getWidth(), currLevel.getHeight());
                // loadLevel(gui.tileListSelected);
            }
        }

        gui.tbWidthEditMode = !gui.tbWidthEditMode;
    }

    if (GuiTextBox({ 10 + 125 + 10, 50 + 30 + 10 + 20, 125, 50 }, gui.tbHeight, 64, gui.tbHeightEditMode)) {
        if (gui.tbHeightEditMode) {
            if (isNumber(gui.tbHeight, sizeof(gui.tbHeight))) {
                // currLevel.setHeight(std::stoi(gui.tbHeight));
                currLevel.changeLevelDimensions(currLevel.getWidth(), std::stoi(gui.tbHeight));
                // levelRepository.changeLevelDimensions(gui.tileListSelected, currLevel.getWidth(), currLevel.getHeight());
                // levelRepository.getLevel(gui.tileListSelected).changeLevelDimensions(currLevel.getWidth(), currLevel.getHeight());
                // loadLevel(gui.tileListSelected);
            }

        }

        gui.tbHeightEditMode = !gui.tbHeightEditMode;
    }

    if (GuiButton({ 10, 200, 220, 60 }, "Add new")) {
        for(int i=0; i<currLevel.getLevelSize(); i++) {currLevel.set(i, 0);};
        createTileNames();
        buildEdges();

        saveLevel("level");

        gui.changeSelected(gui.tileList.size() - 1);
    }

    if (GuiButton({ 10 + 10 + 220 , 200, 220, 60 }, "Save All")) {
        // saveLevel("level");
        levelRepository.saveAll();
        // std::cout<<"SAVE!"<<std::endl;
    }

    if (GuiListViewEx({ 10, 300, 300, 200 }, gui.tileList.data(), static_cast<int>(gui.tileList.size()), &gui.tileListScroll, &gui.tileListSelected, &gui.tileListHovered)) {
        // std::cout<<"LIST: "<<gui.tileListScroll<<std::endl;
        if (gui.tileListSelected >=0 && gui.tileListSelected < levelRepository.getLevelCount()) {
            loadLevel(gui.tileListSelected);
        } else {
            std::cerr<<"ERROR [LevelEditor::drawGUI] LOAD! index: "<<gui.tileListSelected<<std::endl;
        }
        // std::cout<<"LOAD!"<<gui.tileListSelected<<std::endl;

    }

    if (GuiButton({ 10 + 10 + 220, 300 + 210, 220, 60 }, "Delete")){
        bool isOk = true;
        if (gui.tileListSelected >=0 && gui.tileListSelected < levelRepository.getLevelCount()) {
            levelRepository.deleteLevel(gui.tileListSelected);
        } else {
            std::cerr<<"ERROR [LevelEditor::drawGUI] DELETE! index: "<<gui.tileListSelected<<std::endl;
            isOk = false;
        }

        int selected = gui.tileListSelected;

        createTileNames();

        if (selected >= gui.tileList.size()) {
            selected = gui.tileList.size() - 1;
        } 

        gui.changeSelected(selected);
        loadLevel(selected);
    }
    
    if (GuiButton({ 10, 300 + 210, 220, 60 }, "Save")){
        if (gui.tileListSelected >=0 && gui.tileListSelected < levelRepository.getLevelCount()) {
            levelRepository.editLevel(gui.tileListSelected, currLevel);
        } else {
            std::cout<<"ERROR [LevelEditor::drawGUI] LOAD! index: "<<gui.tileListSelected<<std::endl;
        }

        createTileNames();
    }

    if (GuiButton({ 10, 300 + 210 + 10 + 60, 220, 60 }, "Save as new")) {
        saveLevel("level");
        // std::cout<<"SAVE!"<<std::endl;

    }

    for(int i=currLevel.getEdgesSize(); i>0; i--) {
        Edge& e = currLevel.getEdge(i-1);
        std::string str = "EDGE ";
        str += std::to_string(i) + " (" + std::to_string(e.tilesCount) + ")"; 
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
    for(int j=0; j<currLevel.getEdgesSize(); j++) {
        Edge& e = currLevel.getEdge(j);
        Color color = RED;
        if (e.assignedNextLevel != -1) color = GREEN;
        DrawLineEx({(float)e.start.x*tileSize,(float) e.start.y*tileSize}, {(float)e.end.x*tileSize,(float) e.end.y*tileSize},4, color);
        std::string str = "EDGE ";
        if (e.type == TOP) str += "TOP ";
        else if (e.type == DOWN) str += "DOWN ";
        else if (e.type == LEFT) str += "LEFT ";
        else  str += "RIGHT ";
        str += std::to_string(i) + " (" + std::to_string(e.tilesCount) + ")"; 
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

    int levelCount = levelRepository.getLevelCount();
    for (int i=0; i<levelCount; i++) {
        gui.tileNames.push_back(std::to_string(i+1) + ". " + levelRepository.getLevel(i).getName());
    }

    gui.rebuildTileList();
}
