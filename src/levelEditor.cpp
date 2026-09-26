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

    nameTextBox = SmallTextBox({10, 50, 260, 50}, [this]() {handleLevelNameInput();}, state.currentLevel.getName());
    guiItems.push_back(&nameTextBox);
    
    widthTextBox = SmallTextBox({10, 50 + 30 + 10 + 20, 125, 50}, [this]() {handleLevelDimensionsInput();} , std::to_string(state.currentLevel.getWidth()));
    guiItems.push_back(&widthTextBox);

    heightTextBox = SmallTextBox({10 + 125 + 10, 50 + 30 + 10 + 20, 125, 50}, [this]() {handleLevelDimensionsInput();}, std::to_string(state.currentLevel.getHeight()));
    guiItems.push_back(&heightTextBox);

    addNewButton = Button({10, 200,220, 60}, "Add new", [this]() {handleAddNewLevel(); });
    guiItems.push_back(&addNewButton);
    
    saveAllButton = Button({10 + 10 + 220, 200, 220, 60}, "Save all", [this]() {handleSaveAll(); });
    guiItems.push_back(&saveAllButton);

    levelListSelection = ListView({10, 300, 300, 200}, getLevelNames(), [this]() { handleLevelListSelection(); });
    guiItems.push_back(&levelListSelection);

    deleteButton = Button({10 + 10 + 220, 300 + 210, 220, 60}, "Delete", [this]() {handleLevelDelete(); });
    guiItems.push_back(&deleteButton);

    saveButton = Button({10, 300 + 210, 220, 60}, "Edit", [this]() {handleLevelSave(); });
    guiItems.push_back(&saveButton);

    saveAsNewButton = Button({10, 300 + 210 + 10 + 60, 220, 60}, "Save as new", [this]() {handleSaveAsNew(); });
    guiItems.push_back(&saveAsNewButton);

    edgeAssignmentDropDown = TextLabelAndDropDownBoxList({10, 300 + 210 + 10 + 60 + 80}, {});
    guiItems.push_back(&edgeAssignmentDropDown);
    

    syncCurrentLevelUi();
    // refreshLevelUiState();

    // createEdgeDropDownBoxes();
}

LevelEditor::LevelEditor(LevelRepository& tileLevelManager) : LevelEditor(tileLevelManager, LevelEditorState::DefaultTileSize)
{

}

std::vector<EdgeDropDownRow> buildEdgeDropdownRows(const LevelData& level, const std::vector<std::string>& levelNames)
{
    std::vector<EdgeDropDownRow> rows;
    rows.reserve(level.getEdgesSize());

    for (int i = level.getEdgesSize(); i > 0; --i) {
        const Edge& edge = level.getEdge(i - 1);
        EdgeDropDownRow row;
        row.edgeIndex = i - 1;
        row.label = "EDGE " + std::to_string(i) + " (" + std::to_string(edge.tilesCount) + ")";
        row.options = levelNames;
        row.selectedIndex = edge.assignedNextLevel;
        rows.push_back(row);
    }

    return rows;
}

void LevelEditor::syncCurrentLevelUi()
{
    snprintf(widthTextBox.value, sizeof(widthTextBox.value), std::to_string(state.currentLevel.getWidth()).c_str());
    snprintf(heightTextBox.value, sizeof(heightTextBox.value), std::to_string(state.currentLevel.getHeight()).c_str());
    snprintf(nameTextBox.value, sizeof(nameTextBox.value), state.currentLevel.getName().c_str());

    std::vector<std::string> levelNames = getLevelNames();
    auto rows = buildEdgeDropdownRows(state.currentLevel, levelNames);
    edgeAssignmentDropDown.update(rows, [this](int edgeIndex, int selectedIndex) {
        if (edgeIndex >= 0 && edgeIndex < state.currentLevel.getEdgesSize()) {
            state.currentLevel.getEdge(edgeIndex).assignedNextLevel = selectedIndex;
        }
    });
    levelListSelection.setTiles(levelNames);
}

void LevelEditor::saveLevel(std::string name) {
    state.currentLevel.setName(name + std::to_string(levelRepository.getLevelCount()));

    levelRepository.saveLevel(state.currentLevel);

    syncCurrentLevelUi();
    // refreshLevelUiState();
}

void LevelEditor::loadLevel(int levelID) {
    LevelData levelData = levelRepository.getLevel(levelID);

    state.camera.offset.x = state.screenWidth/2 - (levelData.getWidth()*state.tileSize)/2;   // coordinate x origin
    state.camera.offset.y = state.screenHeight/2 - (levelData.getHeight()*state.tileSize)/2;   // coordinate y origin

    state.currentLevel = levelData;
    std::cout<<"LEVELS: "<<state.currentLevel.getLevelSize()<<", EDGES: "<<state.currentLevel.getEdgesSize()<<std::endl;

    syncCurrentLevelUi();
    // refreshLevelUiState();
    // createEdgeDropDownBoxes();

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
    if (!nameTextBox.getEditMode()) {
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

            syncCurrentLevelUi();

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

bool isNumber(std::string str) {
    for(int i=0; i<str.size() && str[i] != '\0'; i++) {
        if (!isdigit(str[i])) return false;
    }
    return true;

}

void LevelEditor::drawGUI()
{

    // drawEdgeDropdowns();

    for(auto& item : guiItems) item->draw();
    // for(auto& item : textLabels) item.draw();
    // for(auto& item : edgeDropDownBoxes) item.draw();
}

// void LevelEditor::createEdgeDropDownBoxes()
// {
//     textLabels.clear();
//     textLabels.reserve(state.currentLevel.getEdgesSize());
//     edgeDropDownBoxes.clear();
//     edgeDropDownBoxes.reserve(state.currentLevel.getEdgesSize());
//     for(int i=state.currentLevel.getEdgesSize(); i>0; i--) {
//         Edge& e = state.currentLevel.getEdge(i-1);
//         std::string str = "EDGE ";
//         str += std::to_string(i) + " (" + std::to_string(e.tilesCount) + ")";
//         Vector2 textPos = {10, (i-1)*30 + 300 + 210 + 10 + 60 + 80};
//         DrawText(str.c_str(), textPos.x, textPos.y, 20, BLACK);
//         int textWidth = MeasureText(str.c_str(), 20);
        
//         textLabels.push_back(TextLabel(textPos, str, 20, BLACK));
//         edgeDropDownBoxes.push_back(DropDownBox({ textPos.x + textWidth + 10, textPos.y, 200, 25 }, getLevelNames()));
//         DropDownBox& dropDownBox = edgeDropDownBoxes.back();
//         dropDownBox.selectedIndex = e.assignedNextLevel;
//         edgeDropDownBoxes.back().setOnChange([this, &e, &dropDownBox]() {e.assignedNextLevel = dropDownBox.selectedIndex;});
//     }
// }

void LevelEditor::saveCurrentLevel()
{
    if (levelListSelection.selectedIndex >=0 && levelListSelection.selectedIndex < levelRepository.getLevelCount()) {
        levelRepository.editLevel(levelListSelection.selectedIndex, state.currentLevel);
    } else {
        std::cout<<"ERROR [LevelEditor::saveCurrentLevel] LOAD! index: "<<levelListSelection.selectedIndex<<std::endl;
    }

    syncCurrentLevelUi();
    // refreshLevelUiState();
    // edgeAssignmentDropDown.update(state.currentLevel, getLevelNames());
    // levelListSelection.setTiles(getLevelNames());
}

void LevelEditor::loadSelectedLevel()
{
    if (levelListSelection.selectedIndex >=0 && levelListSelection.selectedIndex < levelRepository.getLevelCount()) {
        loadLevel(levelListSelection.selectedIndex);
    } else {
        std::cerr<<"ERROR [LevelEditor::loadSelectedLevel] LOAD! index: "<<levelListSelection.selectedIndex<<std::endl;
    }
}

void LevelEditor::deleteSelectedLevel()
{
    bool isOk = true;
    int levelCountBefore = levelRepository.getLevelCount();
    if (levelListSelection.selectedIndex >=0 && levelListSelection.selectedIndex < levelRepository.getLevelCount()) {
        levelRepository.deleteLevel(levelListSelection.selectedIndex);
        // levelListSelection.setTiles(getLevelNames());
    } else {
        std::cerr<<"ERROR [LevelEditor::deleteSelectedLevel] DELETE! index: "<<levelListSelection.selectedIndex<<std::endl;
        isOk = false;
    }
    
    syncCurrentLevelUi();

    int selected = levelListSelection.selectedIndex;

    // std::cout<<"[delete] selected: "<<selected<<", levels size before: "<<levelCountBefore<<", after"<<levelRepository.getLevelCount()<<std::endl;
    
    if (selected >= levelListSelection.getTilesSize()) {
        selected = levelListSelection.getTilesSize() - 1;
    }
    
    levelListSelection.changeSelected(selected);

    if (isOk) {
        loadLevel(levelListSelection.selectedIndex);
    }

    // refreshLevelUiState();
}

void LevelEditor::createNewLevel()
{
    for(int i=0; i<state.currentLevel.getLevelSize(); i++) {state.currentLevel.set(i, 0);};

    saveLevel("level");

    levelListSelection.changeSelected(levelListSelection.getTilesSize() - 1);
}

void LevelEditor::saveAllLevels()
{
    levelRepository.saveAll();
}

void LevelEditor::applyLevelDimensionsFromUi()
{
    if (isNumber(widthTextBox.value, sizeof(widthTextBox.value))) {
        state.currentLevel.changeLevelDimensions(std::stoi(widthTextBox.value), state.currentLevel.getHeight());
    }

    if (isNumber(heightTextBox.value, sizeof(heightTextBox.value))) {
        state.currentLevel.changeLevelDimensions(state.currentLevel.getWidth(), std::stoi(heightTextBox.value));
    }
}

void LevelEditor::handleLevelNameInput()
{
    state.currentLevel.setName(nameTextBox.value);
}

void LevelEditor::handleLevelDimensionsInput()
{
    applyLevelDimensionsFromUi();
}

void LevelEditor::handleAddNewLevel()
{
    createNewLevel();
}

void LevelEditor::handleSaveAll()
{
    saveAllLevels();
}

void LevelEditor::handleLevelListSelection()
{
    loadSelectedLevel();
}

void LevelEditor::handleLevelDelete()
{
    deleteSelectedLevel();
}

void LevelEditor::handleLevelSave()
{
    saveCurrentLevel();
}

void LevelEditor::handleSaveAsNew()
{
    saveLevel("level");
}

// void LevelEditor::drawEdgeDropdowns()
// {
//     for(int i=state.currentLevel.getEdgesSize(); i>0; i--) {
//         Edge& e = state.currentLevel.getEdge(i-1);
//         std::string str = "EDGE ";
//         str += std::to_string(i) + " (" + std::to_string(e.tilesCount) + ")";
//         Vector2 textPos = {10, (i-1)*30 + 300 + 210 + 10 + 60 + 80};
//         DrawText(str.c_str(), textPos.x, textPos.y, 20, BLACK);
//         int textWidth = MeasureText(str.c_str(), 20);
//         if (GuiDropdownBox({ textPos.x + textWidth + 10, textPos.y, 200, 25 }, gui.dropDownOptions.c_str(), &gui.dropDowns[i-1].dropdownActive, gui.dropDowns[i-1].dropDownEditMode)) {
//             gui.dropDowns[i-1].dropDownEditMode = !gui.dropDowns[i-1].dropDownEditMode;
//             e.assignedNextLevel = gui.dropDowns[i-1].dropdownActive;
//         }
//     }
    
// }

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

std::vector<std::string> LevelEditor::getLevelNames()
{
    std::vector<std::string> levelNames;

    int levelCount = levelRepository.getLevelCount();
    for (int i=0; i<levelCount; i++) {
        levelNames.push_back(std::to_string(i+1) + ". " + levelRepository.getLevel(i).getName());
    }

    return levelNames;
}
