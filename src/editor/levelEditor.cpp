#include "levelEditor.hpp"

#include "raylib.h"
#include "raymath.h"
#include <iostream>
#include <string>
#include <cstring>
#include "third_party/raygui.h"
#include "level/levelMap.hpp"

LevelEditor::LevelEditor(LevelRepository& levelRepository,Assets& assets, std::function<void()> onSwitchToGame, std::function<void()> onSwitchToMenu)
: levelRepository(levelRepository),
  assets(assets),
  m_onSwitchToGame(onSwitchToGame),
  m_onSwitchToMenu(onSwitchToMenu)
{
    state.currentLevel = LevelData(LevelEditorState::DefaultLevelWidth, LevelEditorState::DefaultLevelHeight);

    state.camera.offset = calculateCameraCenterOffset({state.screenWidth, state.screenHeight}, {state.currentLevel.getWidth(), state.currentLevel.getHeight()}, state.getTileSize());
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

    tileSizeTextBox = SmallTextBox({10 + 125 + 10 + 10 + 125, 50 + 30 + 10 + 20, 60, 50}, [this]() {state.currentLevel.setTileSize(std::stoi(tileSizeTextBox.value));syncCurrentLevelUi();}, std::to_string(state.currentLevel.getTileSize()));
    guiItems.push_back(&tileSizeTextBox);

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
}


std::vector<EdgeDropDownRow> buildEdgeDropdownRows(const LevelData& level, const std::vector<std::string>& levelNames)
{
    std::vector<EdgeDropDownRow> rows;
    rows.reserve(level.getEdgesSize());

    for (int i = level.getEdgesSize(); i > 0 ; i--) {
        const Edge& edge = level.getEdge(i-1);
        EdgeDropDownRow row;
        row.edgeIndex = i-1;
        row.label = "EDGE " + std::to_string(i) + " (" + std::to_string(edge.tilesCount) + ")";
        row.options = levelNames;
        row.assignedLevel = edge.assignedNextLevel;
        rows.push_back(row);
    }

    return rows;
}

void LevelEditor::syncCurrentLevelUi()
{
    snprintf(widthTextBox.value, sizeof(widthTextBox.value), "%s", std::to_string(state.currentLevel.getWidth()).c_str());
    snprintf(heightTextBox.value, sizeof(heightTextBox.value),"%s", std::to_string(state.currentLevel.getHeight()).c_str());
    snprintf(nameTextBox.value, sizeof(nameTextBox.value), "%s",state.currentLevel.getName().c_str());
    snprintf(tileSizeTextBox.value, sizeof(tileSizeTextBox.value),"%s", std::to_string(state.currentLevel.getTileSize()).c_str());

    std::vector<std::string> levelNames = getLevelNames();
    auto rows = buildEdgeDropdownRows(state.currentLevel, levelNames);
    edgeAssignmentDropDown.update(rows, [this](int edgeIndex, int selectedIndex) {
        if (edgeIndex >= 0 && edgeIndex < state.currentLevel.getEdgesSize()) {
            const auto& edge = state.currentLevel.getEdge(edgeIndex);
            state.currentLevel.setLink(edge.getKey(), selectedIndex);
        }
    });
    levelListSelection.setTiles(levelNames);
}

void LevelEditor::saveLevel(std::string name) {
    state.currentLevel.setName(name + std::to_string(levelRepository.getLevelCount()));

    levelRepository.saveLevel(state.currentLevel);

    syncCurrentLevelUi();
}

void LevelEditor::loadLevel(int levelID) {
    LevelData levelData = levelRepository.getLevel(levelID);

    state.camera.offset = calculateCameraCenterOffset({state.screenWidth, state.screenHeight}, {state.currentLevel.getWidth(), state.currentLevel.getHeight()}, state.getTileSize());

    state.currentLevel = levelData;

    syncCurrentLevelUi();
}

void LevelEditor::processInput()
{
    if (!nameTextBox.getEditMode()) {
        if (IsKeyPressed(KEY_ESCAPE)) {
            m_onSwitchToMenu();
        }

        if (IsKeyPressed(KEY_E)) {
            m_onSwitchToGame();
        }   

        if (IsKeyPressed(KEY_R)) {
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
        int xPos = static_cast<int>((mousePos.x - state.camera.offset.x)/state.getTileSize());
        int yPos = static_cast<int>((mousePos.y - state.camera.offset.y)/state.getTileSize());

        if (xPos >= 0 && xPos < state.currentLevel.getWidth() && yPos >=0 && yPos < state.currentLevel.getHeight()) {
            int tileIndex = yPos*state.currentLevel.getWidth() + xPos;

            if (firstTime) {
                if (state.currentLevel.get(tileIndex) == state.selectedTile) state.mouseMode = 1;
                else state.mouseMode = 0;
            }
            
            if (state.mouseMode == 0) {
                int offsetX = 0;
                int offsetY = 0;
                for(int i=0; i<state.selectedTiles.size(); i++) {
                    int _tileIndex = (yPos+offsetY)*state.currentLevel.getWidth() + (xPos+offsetX);
                    state.currentLevel.set(_tileIndex, state.selectedTiles[i]);
                    if (i+1 <state.selectedTiles.size() && state.selectedTiles[i+1] - state.selectedTiles[i] != 1) {
                        offsetX = -1;
                        offsetY++;
                    }
                    offsetX++;
                }
            }
            else
                state.currentLevel.set(tileIndex, -1);

            syncCurrentLevelUi();

        } 
    }
}

void LevelEditor::update([[maybe_unused]] float deltaTime)
{
}

void LevelEditor::draw()
{
    ClearBackground(LIGHTGRAY);

    GuiSetStyle(DEFAULT, TEXT_SIZE, GuiGetFont().baseSize*3);
    BeginMode2D(state.camera);

    drawLevel(state.currentLevel, assets, assets.getBackgroundGroup(), true);
    
    Vector2 mousePos = GetMousePosition();
    if (state.mouseMode == 0 && mousePos.x - state.camera.offset.x >= 0 && mousePos.x - state.camera.offset.x < state.currentLevel.getWidth()*state.getTileSize() && mousePos.y - state.camera.offset.y >= 0 && mousePos.y - state.camera.offset.y < state.currentLevel.getHeight()*state.getTileSize()) { 
        int offsetX = 0;
        int offsetY = 0;
        for(int i=0; i<state.selectedTiles.size(); i++) {
            Vector2 pos = {(floor((mousePos.x-state.camera.offset.x)/state.getTileSize()) + offsetX)*state.getTileSize(), (floor((mousePos.y-state.camera.offset.y)/state.getTileSize())+offsetY)*state.getTileSize()};
            assets.getSprite(state.selectedTiles[i]).draw(pos, {static_cast<float>(state.getTileSize()), static_cast<float>(state.getTileSize())});
            if (i+1 <state.selectedTiles.size() && state.selectedTiles[i+1] - state.selectedTiles[i] != 1) {
                offsetX = -1;
                offsetY++;
            }
            offsetX++;
        }
    }

    for(int j=0; j<state.currentLevel.getWidth()+1; j++) {
        Vector2 start = {static_cast<float>(j*state.getTileSize()), 0.0f};
        Vector2 end = {static_cast<float>(j*state.getTileSize()), state.currentLevel.getHeight()*static_cast<float>(state.getTileSize())};
        DrawLineEx(start, end, 1.0f, BLACK);
    }

    for(int i=0; i<state.currentLevel.getHeight()+1; i++) {
        Vector2 start = {0.0f, static_cast<float>(i*state.getTileSize())};
        Vector2 end = {state.currentLevel.getWidth()*static_cast<float>(state.getTileSize()), static_cast<float>(i*state.getTileSize())};
        DrawLineEx(start, end, 1.0f, BLACK);
    }

    
    drawEdges();
    
    EndMode2D();

    drawGUI();

    drawSprites();
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

    for(auto& item : guiItems) item->draw();
}

void LevelEditor::saveCurrentLevel()
{
    if (levelListSelection.selectedIndex >=0 && levelListSelection.selectedIndex < levelRepository.getLevelCount()) {
        levelRepository.editLevel(levelListSelection.selectedIndex, state.currentLevel);
    } else {
        std::cout<<"ERROR [LevelEditor::saveCurrentLevel] LOAD! index: "<<levelListSelection.selectedIndex<<std::endl;
    }

    syncCurrentLevelUi();
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
    if (levelListSelection.selectedIndex >=0 && levelListSelection.selectedIndex < levelRepository.getLevelCount()) {
        levelRepository.deleteLevel(levelListSelection.selectedIndex);
    } else {
        std::cerr<<"ERROR [LevelEditor::deleteSelectedLevel] DELETE! index: "<<levelListSelection.selectedIndex<<std::endl;
        isOk = false;
    }
    
    syncCurrentLevelUi();

    int selected = levelListSelection.selectedIndex;
    
    if (selected >= levelListSelection.getTilesSize()) {
        selected = static_cast<int>(levelListSelection.getTilesSize()) - 1;
    }
    
    levelListSelection.changeSelected(selected);

    if (isOk) {
        loadLevel(levelListSelection.selectedIndex);
    }
}

void LevelEditor::createNewLevel()
{
    for(int i=0; i<state.currentLevel.getLevelSize(); i++) {state.currentLevel.set(i, -1);};

    saveLevel("level");

    levelListSelection.changeSelected(static_cast<int>(levelListSelection.getTilesSize()) - 1);
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


void LevelEditor::drawEdges()
{
    int i=1;
    for(int j=0; j<state.currentLevel.getEdgesSize(); j++) {
        Edge& e = state.currentLevel.getEdge(j);
        int tileSize = state.currentLevel.getTileSize();
        Color color = RED;
        if (e.assignedNextLevel != -1) color = GREEN;
        DrawLineEx({static_cast<float>(e.start.x)*tileSize, static_cast<float>(e.start.y)*tileSize}, {static_cast<float>(e.end.x)*tileSize, static_cast<float>(e.end.y)*tileSize}, 4.0f, color);
        std::string str = "EDGE ";
        if (e.type == EdgeType::TOP) str += "TOP ";
        else if (e.type == EdgeType::DOWN) str += "DOWN ";
        else if (e.type == EdgeType::LEFT) str += "LEFT ";
        else  str += "RIGHT ";
        str += std::to_string(i) + " (" + std::to_string(e.tilesCount) + ")"; 
        Vector2 textPos = {static_cast<float>(e.start.x)*tileSize, static_cast<float>(e.start.y)*tileSize};
        int textWidth = MeasureText(str.c_str(), 20);
        if (e.type == EdgeType::TOP) {
            textPos.y -= 20; 
        } else if (e.type == EdgeType::LEFT) {
            textPos.x -= textWidth - 1;
        }
        DrawText(str.c_str(),static_cast<int>(textPos.x),static_cast<int>(textPos.y), 20, BLACK);
        i++;
    }
}

Rectangle rectAbs(const Rectangle& rect) {
    if (rect.width >= 0 && rect.height >= 0) {
        return rect;
    }

    if (rect.width <= 0 && rect.height >= 0) {
        Vector2 start = {rect.x + rect.width, rect.y};
        return {start.x, start.y, abs(rect.width), rect.height};
    }

    if (rect.width >= 0 && rect.height <= 0) {
        Vector2 start = {rect.x, rect.y + rect.height};
        return {start.x, start.y, abs(rect.width), abs(rect.height)};
        
    }

    if (rect.width <= 0 && rect.height <= 0) {
        Vector2 start = {rect.x + rect.width, rect.y + rect.height};
        return {start.x, start.y, abs(rect.width), abs(rect.height)};
    }

    return rect;
}

void drawRectLines(Rectangle rect, float thick, Color color ) {
    if (rect.height < 3 && rect.height >= 0) DrawLine(static_cast<int>(rect.x), static_cast<int>(rect.y), static_cast<int>(rect.x + rect.width), static_cast<int>(rect.y + rect.height), RED);

    DrawRectangleLinesEx(rectAbs(rect), thick, color);
}

void LevelEditor::drawSprites()
{

    int spriteSize = 32;
    Vector2 position = {0.0f,300.0f + 210.0f + 10.0f + 60.0f + 80.0f + static_cast<float>(edgeAssignmentDropDown.getCount())*35.0f + static_cast<float>(state.mouseWheel)*20.0f};
    Vector2 startPosition = {0.0f,300.0f + 210.0f + 10.0f + 60.0f + 80.0f + static_cast<float>(edgeAssignmentDropDown.getCount())*35.0f};
    Rectangle selectionArea = {startPosition.x, startPosition.y, static_cast<float>(assets.getTextureWidth())*spriteSize, static_cast<float>(assets.getTextureHeight())*spriteSize};

    if (CheckCollisionPointRec(GetMousePosition(), selectionArea)) {
        state.mouseWheel += static_cast<int>(GetMouseWheelMove());
    }
    std::optional<Rectangle> selected;
    std::optional<Rectangle> hover;
    int hoverNum = -1;

    for(int i=0; i<assets.getSpritesSize(); i++) {
        
        if (i%(assets.getTextureWidth()) == 0 && i != 0) {
            position.y += spriteSize;
            position.x = 0;
        }
        
        Sprite sprite = assets.getSprite(i);

        if (position.y >= selectionArea.y) {
            if (i == state.selectedTile) {
                int border = 3;
                selected = {position.x-border, position.y-border, static_cast<float>(spriteSize + 2*border), static_cast<float>(spriteSize + 2*border)};
            }
            if (CheckCollisionPointRec(GetMousePosition(), {position.x, position.y, static_cast<float>(spriteSize), static_cast<float>(spriteSize)})) {
                int border = 3;
                hover = {position.x-border, position.y-border, static_cast<float>(spriteSize + 2*border), static_cast<float>(spriteSize + 2*border)};
                hoverNum = i;

                if (IsMouseButtonDown(0)) {
                    state.selectedTile = i;
                }
            }
        
        sprite.draw(position, {static_cast<float>(spriteSize), static_cast<float>(spriteSize)});

        }

        position.x += spriteSize;
        
    }

    if (selected)
        DrawRectangleLinesEx(*selected, 2, BLACK);

    if (hover) {
        DrawRectangleLinesEx(*hover, 2, GRAY);
        DrawText(std::to_string(hoverNum).c_str(),static_cast<int>((*hover).x+10), static_cast<int>((*hover).y), 32, BLUE);
    }

    if (CheckCollisionPointRec(GetMousePosition(), selectionArea)) {
        if (IsMouseButtonDown(0) && state.isSelection == false) {
            state.startSelection = GetMousePosition();
            state.isSelection = true;
        } 
        if (IsMouseButtonDown(0)) {
            state.endSelection = GetMousePosition();
            state.selectedTiles.clear();
            Vector2 startP = {state.startSelection.x, state.startSelection.y - startPosition.y - state.mouseWheel*20};
            Vector2 endP= {state.endSelection.x, state.endSelection.y - startPosition.y - state.mouseWheel*20};
            Rectangle absSelectedArea = rectAbs({startP.x, startP.y, endP.x - startP.x, endP.y - startP.y});


            int startX = static_cast<int>(absSelectedArea.x/32);
            int startY = static_cast<int>(absSelectedArea.y/32);

            int endX= static_cast<int>((absSelectedArea.x+absSelectedArea.width)/32);
            int endY= static_cast<int>((absSelectedArea.y+absSelectedArea.height)/32);

            for(int y=startY; y<=endY; y++) {
                for(int x=startX; x<=endX; x++) {
                    state.selectedTiles.push_back(y*assets.getTextureWidth() + x);
                }
            }

        } else {
            state.isSelection = false;
        }



    } else {
        state.isSelection = false;
    }

    for(int i=0; i<state.selectedTiles.size(); i++) {
        Rectangle rect = {static_cast<float>(state.selectedTiles[i] % assets.getTextureWidth()) * spriteSize, static_cast<float>(state.selectedTiles[i] / assets.getTextureWidth()) * spriteSize, static_cast<float>(spriteSize), static_cast<float>(spriteSize)};
        if (rect.y + startPosition.y + state.mouseWheel*20 >= selectionArea.y )
            DrawRectangleLinesEx({rect.x + startPosition.x, rect.y + startPosition.y + state.mouseWheel*20, rect.width, rect.height}, 2, GRAY);
    }

    DrawRectangleLines(static_cast<int>(selectionArea.x), static_cast<int>(selectionArea.y), static_cast<int>(selectionArea.width), static_cast<int>(selectionArea.height), BLACK);
    
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
