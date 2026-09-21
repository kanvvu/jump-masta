#include "levelEditor.hpp"

#include "raylib.h"
#include "raymath.h"
#include <iostream>
#include <string>
#include <cstring>

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

LevelEditor::LevelEditor(LevelRepository& levelRepository, int tileSize) : levelRepository(levelRepository)
{
    state.currentLevel = LevelData(LevelEditorState::DefaultLevelWidth, LevelEditorState::DefaultLevelHeight);
    state.tileSize = tileSize;

    state.camera.offset.x = state.screenWidth/2 - (state.currentLevel.getWidth()*state.tileSize)/2;   // coordinate x origin
    state.camera.offset.y = state.screenHeight/2 - (state.currentLevel.getHeight()*state.tileSize)/2;   // coordinate y origin
    state.camera.target.x = 0;         // rotation and zoom x origin (from offset above)
    state.camera.target.y = 0;         // rotation and zoom y origin (from offset above)
    state.camera.rotation = 0;        // rotation in deg
    state.camera.zoom     = 1.0f;      // magnification, i.e fov or zoom

    syncCurrentLevelUi();
    refreshLevelUiState();
}

LevelEditor::LevelEditor(LevelRepository& tileLevelManager) : LevelEditor(tileLevelManager, LevelEditorState::DefaultTileSize)
{

}

void LevelEditor::syncCurrentLevelUi()
{
    snprintf(gui.tbWidth, sizeof(gui.tbWidth), std::to_string(state.currentLevel.getWidth()).c_str());
    snprintf(gui.tbHeight, sizeof(gui.tbHeight), std::to_string(state.currentLevel.getHeight()).c_str());
    snprintf(gui.tbName, sizeof(gui.tbName), state.currentLevel.getName().c_str());
}

void LevelEditor::refreshLevelUiState()
{
    syncCurrentLevelUi();
    createTileNames();
    gui.rebuildDropDowns();
    buildEdges();
}

void LevelEditor::buildEdges()
{
    gui.dropDowns.resize(static_cast<size_t>(state.currentLevel.getEdgesSize()));

    for (int i = 0; i < state.currentLevel.getEdgesSize(); ++i) {
        gui.dropDowns[i].dropdownActive = state.currentLevel.getEdge(i).assignedNextLevel;
    }
}

void LevelEditor::saveLevel(std::string name) {
    state.currentLevel.setName(name + std::to_string(levelRepository.getLevelCount()));

    levelRepository.saveLevel(state.currentLevel);

    refreshLevelUiState();
}

void LevelEditor::loadLevel(int levelID) {
    LevelData levelData = levelRepository.getLevel(levelID);

    state.camera.offset.x = state.screenWidth/2 - (levelData.getWidth()*state.tileSize)/2;   // coordinate x origin
    state.camera.offset.y = state.screenHeight/2 - (levelData.getHeight()*state.tileSize)/2;   // coordinate y origin

    state.currentLevel = levelData;
    std::cout<<"LEVELS: "<<state.currentLevel.getLevelSize()<<", EDGES: "<<state.currentLevel.getEdgesSize()<<std::endl;

    syncCurrentLevelUi();
    refreshLevelUiState();

    // std::vector<Edge> assigned = levelData.edges; 
    // if (assigned.size() != state.currentLevel.edges.size()) {
    //     std::cerr<<"\033[31m"<<"ERROR [LevelEditor::loadLevel] assigned not equal edges!\033[0m\n";
    // } else {
    //     for(int i=0; i<state.currentLevel.edges.size(); i++) {
    //         state.currentLevel.edges[i].assignedNextLevel = assigned[i].assignedNextLevel;
    //     }
    // }
}

void LevelEditor::proccessInput(GameState &gameState)
{
    if (!gui.tbNameEditMode) {
        if (IsKeyPressed(KEY_E)) {
            gameState = GAME;
        }   

        if (IsKeyPressed(KEY_R)) {
            // for (int& tile : state.currentLevel.level) tile = 0;
            for(int i=0; i<state.currentLevel.getLevelSize(); i++) {state.currentLevel.set(i, 0);};
        }

        if (IsKeyPressed(KEY_S)) {
            saveLevel("level");
        }
    }

    if (IsMouseButtonDown(1)) {
        state.camera.offset = Vector2Add(state.camera.offset, GetMouseDelta());
    }

    bool firstTime = false;
    if (IsMouseButtonDown(0) && state.isMouseDown == false) {
        state.isMouseDown = true;
        firstTime = true;
    }

    if (!IsMouseButtonDown(0)) state.isMouseDown = false;


    if (IsMouseButtonDown(0)) {
        Vector2 mousePos = GetMousePosition();
        int xPos = (mousePos.x - state.camera.offset.x)/state.tileSize;
        int yPos = (mousePos.y - state.camera.offset.y)/state.tileSize;

        if (xPos >= 0 && xPos < state.currentLevel.getWidth() && yPos >=0 && yPos < state.currentLevel.getHeight()) {
            int tileIndex = yPos*state.currentLevel.getWidth() + xPos;
            // std::cout<<"TILE INDEX: "<<tileIndex <<"\n";
            // std::cout<<"firstTime: "<<firstTime<<", "<<"mouseMode: "<<state.mouseMode <<"\n";

            if (firstTime) {
                if (state.currentLevel.get(tileIndex) == state.selectedTile) state.mouseMode = 1;
                else state.mouseMode = 0;
            }
            
            if (state.mouseMode == 0)
                state.currentLevel.set(tileIndex,1);
            else
                state.currentLevel.set(tileIndex, 0);

            refreshLevelUiState();
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
    BeginMode2D(state.camera);


    state.currentLevel.draw();

    
    Vector2 mousePos = GetMousePosition();
    if (state.mouseMode == 0 && mousePos.x - state.camera.offset.x >= 0 && mousePos.x - state.camera.offset.x < state.currentLevel.getWidth()*state.tileSize && mousePos.y - state.camera.offset.y >= 0 && mousePos.y - state.camera.offset.y < state.currentLevel.getHeight()*state.tileSize) 
        DrawRectangle(floor((mousePos.x-state.camera.offset.x)/state.tileSize)*state.tileSize, floor((mousePos.y-state.camera.offset.y)/state.tileSize)*state.tileSize, state.tileSize, state.tileSize, LIGHTGRAY);
    // std::cout<<"("<<mousePos.x <<", "<<mousePos.y<<"), ";
    // std::cout<<"("<<state.camera.offset.x <<", "<<state.camera.offset.y<<"), ";
    // std::cout<<mousePos.x - state.camera.offset.x <<", "<<mousePos.y - state.camera.offset.y<<"\n";

    
    for(int j=0; j<state.currentLevel.getWidth()+1; j++) {
        Vector2 start = {static_cast<float>(j*state.tileSize), 0.0f};
        Vector2 end = {static_cast<float>(j*state.tileSize), state.currentLevel.getHeight()*static_cast<float>(state.tileSize)};
        DrawLineEx(start, end, 1.0f, BLACK);
    }

    for(int i=0; i<state.currentLevel.getHeight()+1; i++) {
        Vector2 start = {0.0f, static_cast<float>(i*state.tileSize)};
        Vector2 end = {state.currentLevel.getWidth()*static_cast<float>(state.tileSize), static_cast<float>(i*state.tileSize)};
        DrawLineEx(start, end, 1.0f, BLACK);
    }

    
    drawEdges();
    
    EndMode2D();

    drawGUI();
}



bool isNumber(const char* str, int size) {
    for(int i=0; i<size && str[i] != '\0'; i++) {
        if (!isdigit(str[i])) return false;
    }
    return true;

}

void LevelEditor::drawGUI()
{
    handleLevelNameInput();
    handleLevelDimensionsInput();
    handleAddNewLevel();
    handleSaveAll();
    handleLevelListSelection();
    handleLevelDelete();
    handleLevelSave();
    handleSaveAsNew();
    drawEdgeDropdowns();
}

void LevelEditor::executeCommand(EditorCommand command)
{
    switch (command) {
        case EditorCommand::SaveCurrentLevel:
            saveCurrentLevel();
            break;
        case EditorCommand::SaveAllLevels:
            saveAllLevels();
            break;
        case EditorCommand::LoadSelectedLevel:
            loadSelectedLevel();
            break;
        case EditorCommand::DeleteSelectedLevel:
            deleteSelectedLevel();
            break;
        case EditorCommand::CreateNewLevel:
            createNewLevel();
            break;
        case EditorCommand::ApplyLevelDimensions:
            applyLevelDimensionsFromUi();
            break;
    }
}

void LevelEditor::saveCurrentLevel()
{
    if (gui.tileListSelected >=0 && gui.tileListSelected < levelRepository.getLevelCount()) {
        levelRepository.editLevel(gui.tileListSelected, state.currentLevel);
    } else {
        std::cout<<"ERROR [LevelEditor::drawGUI] LOAD! index: "<<gui.tileListSelected<<std::endl;
    }

    refreshLevelUiState();
}

void LevelEditor::loadSelectedLevel()
{
    if (gui.tileListSelected >=0 && gui.tileListSelected < levelRepository.getLevelCount()) {
        loadLevel(gui.tileListSelected);
    } else {
        std::cerr<<"ERROR [LevelEditor::drawGUI] LOAD! index: "<<gui.tileListSelected<<std::endl;
    }
}

void LevelEditor::deleteSelectedLevel()
{
    bool isOk = true;
    if (gui.tileListSelected >=0 && gui.tileListSelected < levelRepository.getLevelCount()) {
        levelRepository.deleteLevel(gui.tileListSelected);
    } else {
        std::cerr<<"ERROR [LevelEditor::drawGUI] DELETE! index: "<<gui.tileListSelected<<std::endl;
        isOk = false;
    }

    int selected = gui.tileListSelected;

    refreshLevelUiState();

    if (selected >= gui.tileList.size()) {
        selected = gui.tileList.size() - 1;
    }

    gui.changeSelected(selected);
    if (isOk) {
        loadLevel(selected);
    }
}

void LevelEditor::createNewLevel()
{
    for(int i=0; i<state.currentLevel.getLevelSize(); i++) {state.currentLevel.set(i, 0);};
refreshLevelUiState();

    saveLevel("level");

    gui.changeSelected(gui.tileList.size() - 1);
}

void LevelEditor::saveAllLevels()
{
    levelRepository.saveAll();
}

void LevelEditor::applyLevelDimensionsFromUi()
{
    if (isNumber(gui.tbWidth, sizeof(gui.tbWidth))) {
        state.currentLevel.changeLevelDimensions(std::stoi(gui.tbWidth), state.currentLevel.getHeight());
    }

    if (isNumber(gui.tbHeight, sizeof(gui.tbHeight))) {
        state.currentLevel.changeLevelDimensions(state.currentLevel.getWidth(), std::stoi(gui.tbHeight));
    }
}

void LevelEditor::handleLevelNameInput()
{
    if (GuiTextBox({ 10, 50, 260, 50 }, gui.tbName, 64, gui.tbNameEditMode)) {
        if (gui.tbNameEditMode) {
            state.currentLevel.setName(gui.tbName);
        }

        gui.tbNameEditMode = !gui.tbNameEditMode;
    }
}

void LevelEditor::handleLevelDimensionsInput()
{
    if (GuiTextBox({ 10, 50 + 30 + 10 + 20, 125, 50 }, gui.tbWidth, 64, gui.tbWidthEditMode)) {
        if (gui.tbWidthEditMode) {
            executeCommand(EditorCommand::ApplyLevelDimensions);
        }

        gui.tbWidthEditMode = !gui.tbWidthEditMode;
    }

    if (GuiTextBox({ 10 + 125 + 10, 50 + 30 + 10 + 20, 125, 50 }, gui.tbHeight, 64, gui.tbHeightEditMode)) {
        if (gui.tbHeightEditMode) {
            executeCommand(EditorCommand::ApplyLevelDimensions);
        }

        gui.tbHeightEditMode = !gui.tbHeightEditMode;
    }
}

void LevelEditor::handleAddNewLevel()
{
    if (GuiButton({ 10, 200, 220, 60 }, "Add new")) {
        executeCommand(EditorCommand::CreateNewLevel);
    }
}

void LevelEditor::handleSaveAll()
{
    if (GuiButton({ 10 + 10 + 220 , 200, 220, 60 }, "Save All")) {
        executeCommand(EditorCommand::SaveAllLevels);
    }
}

void LevelEditor::handleLevelListSelection()
{
    if (GuiListViewEx({ 10, 300, 300, 200 }, gui.tileList.data(), static_cast<int>(gui.tileList.size()), &gui.tileListScroll, &gui.tileListSelected, &gui.tileListHovered)) {
        executeCommand(EditorCommand::LoadSelectedLevel);
    }
}

void LevelEditor::handleLevelDelete()
{
    if (GuiButton({ 10 + 10 + 220, 300 + 210, 220, 60 }, "Delete")){
        executeCommand(EditorCommand::DeleteSelectedLevel);
    }
}

void LevelEditor::handleLevelSave()
{
    if (GuiButton({ 10, 300 + 210, 220, 60 }, "Save")){
        executeCommand(EditorCommand::SaveCurrentLevel);
    }
}

void LevelEditor::handleSaveAsNew()
{
    if (GuiButton({ 10, 300 + 210 + 10 + 60, 220, 60 }, "Save as new")) {
        saveLevel("level");
    }
}

void LevelEditor::drawEdgeDropdowns()
{
    for(int i=state.currentLevel.getEdgesSize(); i>0; i--) {
        Edge& e = state.currentLevel.getEdge(i-1);
        std::string str = "EDGE ";
        str += std::to_string(i) + " (" + std::to_string(e.tilesCount) + ")";
        Vector2 textPos = {10, (i-1)*30 + 300 + 210 + 10 + 60 + 80};
        DrawText(str.c_str(), textPos.x, textPos.y, 20, BLACK);
        int textWidth = MeasureText(str.c_str(), 20);
        if (GuiDropdownBox({ textPos.x + textWidth + 10, textPos.y, 200, 25 }, gui.dropDownOptions.c_str(), &gui.dropDowns[i-1].dropdownActive, gui.dropDowns[i-1].dropDownEditMode)) {
            gui.dropDowns[i-1].dropDownEditMode = !gui.dropDowns[i-1].dropDownEditMode;
            e.assignedNextLevel = gui.dropDowns[i-1].dropdownActive;
        }
    }
}

void LevelEditor::drawEdges()
{
    
    // std::cout<<"EDGES: "<<edges.size()<<std::endl;
    int i=1;
    for(int j=0; j<state.currentLevel.getEdgesSize(); j++) {
        Edge& e = state.currentLevel.getEdge(j);
        Color color = RED;
        if (e.assignedNextLevel != -1) color = GREEN;
        DrawLineEx({(float)e.start.x*state.tileSize,(float) e.start.y*state.tileSize}, {(float)e.end.x*state.tileSize,(float) e.end.y*state.tileSize}, 4.0f, color);
        std::string str = "EDGE ";
        if (e.type == TOP) str += "TOP ";
        else if (e.type == DOWN) str += "DOWN ";
        else if (e.type == LEFT) str += "LEFT ";
        else  str += "RIGHT ";
        str += std::to_string(i) + " (" + std::to_string(e.tilesCount) + ")"; 
        Vector2 textPos = {e.start.x*state.tileSize, e.start.y*state.tileSize};
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
