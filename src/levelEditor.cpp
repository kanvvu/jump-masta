#include "levelEditor.hpp"

#include "raylib.h"
#include "raymath.h"
#include <iostream>
#include <string>
#include <cstring>

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

LevelEditor::LevelEditor(LevelRepository& levelRepository,Assets& assets, int tileSize) : levelRepository(levelRepository), assets(assets)
{
    state.currentLevel = LevelData(LevelEditorState::DefaultLevelWidth, LevelEditorState::DefaultLevelHeight);

    state.camera.offset.x = state.screenWidth/2 - (state.currentLevel.getWidth()*state.getTileSize())/2;   // coordinate x origin
    state.camera.offset.y = state.screenHeight/2 - (state.currentLevel.getHeight()*state.getTileSize())/2;   // coordinate y origin
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

LevelEditor::LevelEditor(LevelRepository& tileLevelManager, Assets& assets) : LevelEditor(tileLevelManager, assets, LevelEditorState::DefaultTileSize)
{

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
    snprintf(widthTextBox.value, sizeof(widthTextBox.value), std::to_string(state.currentLevel.getWidth()).c_str());
    snprintf(heightTextBox.value, sizeof(heightTextBox.value), std::to_string(state.currentLevel.getHeight()).c_str());
    snprintf(nameTextBox.value, sizeof(nameTextBox.value), state.currentLevel.getName().c_str());
    snprintf(tileSizeTextBox.value, sizeof(tileSizeTextBox.value), std::to_string(state.currentLevel.getTileSize()).c_str());

    std::vector<std::string> levelNames = getLevelNames();
    auto rows = buildEdgeDropdownRows(state.currentLevel, levelNames);
    edgeAssignmentDropDown.update(rows, [this](int edgeIndex, int selectedIndex) {
        if (edgeIndex >= 0 && edgeIndex < state.currentLevel.getEdgesSize()) {
            std::cout<<"\tedge: "<<edgeIndex<<", index: "<<selectedIndex<<"\n";
            state.currentLevel.getEdge(edgeIndex).assignedNextLevel = selectedIndex;
        } else {
            std::cout<<"lol edgeAssignmentDropDown lol!!!\n";
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

    state.camera.offset.x = state.screenWidth/2 - (levelData.getWidth()*levelData.getTileSize())/2;   // coordinate x origin
    state.camera.offset.y = state.screenHeight/2 - (levelData.getHeight()*levelData.getTileSize())/2;   // coordinate y origin

    state.currentLevel = levelData;
    // std::cout<<"LEVELS: "<<state.currentLevel.getLevelSize()<<", EDGES: "<<state.currentLevel.getEdgesSize()<<std::endl;

    syncCurrentLevelUi();
}

void LevelEditor::proccessInput(GameState &gameState)
{
    if (!nameTextBox.getEditMode()) {
        if (IsKeyPressed(KEY_E)) {
            gameState = GAME;
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
        int xPos = (mousePos.x - state.camera.offset.x)/state.getTileSize();
        int yPos = (mousePos.y - state.camera.offset.y)/state.getTileSize();

        if (xPos >= 0 && xPos < state.currentLevel.getWidth() && yPos >=0 && yPos < state.currentLevel.getHeight()) {
            int tileIndex = yPos*state.currentLevel.getWidth() + xPos;

            if (firstTime) {
                if (state.currentLevel.get(tileIndex) == state.selectedTile) state.mouseMode = 1;
                else state.mouseMode = 0;
            }
            
            if (state.mouseMode == 0) {
                // state.currentLevel.set(tileIndex, state.selectedTile);
                int offsetX = 0;
                int offsetY = 0;
                for(int i=0; i<state.selectedTiles.size(); i++) {
                    int tileIndex = (yPos+offsetY)*state.currentLevel.getWidth() + (xPos+offsetX);
                    state.currentLevel.set(tileIndex, state.selectedTiles[i]);
                    if (i+1 <state.selectedTiles.size() && state.selectedTiles[i+1] - state.selectedTiles[i] != 1) {
                        offsetX = -1;
                        offsetY++;
                    }
                    offsetX++;
                }
            }
            else
                state.currentLevel.set(tileIndex, 0);

            syncCurrentLevelUi();

        } 
    }
}

void LevelEditor::update(float deltaTime)
{
}

void LevelEditor::draw()
{
    ClearBackground(LIGHTGRAY);

    GuiSetStyle(DEFAULT, TEXT_SIZE, GuiGetFont().baseSize*3);
    BeginMode2D(state.camera);


    int m_widthCount =  state.currentLevel.getWidth();
    int m_heightCount = state.currentLevel.getHeight();
    int m_tileSize = state.currentLevel.getTileSize();

    for(int i=0; i<m_heightCount; i++) {
        for(int j=0; j<m_widthCount; j++) {
            int tileIndex = i*m_widthCount+ j;
            int xPos = j*m_tileSize;
            int yPos = i*m_tileSize;
            Color tileColor = WHITE;
            // DrawRectangle(xPos, yPos, m_tileSize, m_tileSize, tileColor);

            int tile = state.currentLevel.get(tileIndex);
            // if (tile == 1) {
            //     tileColor = BLUE;
            //     grass.draw({static_cast<float>(xPos),static_cast<float>(yPos)});
            // }
            if (tile != 0)
                assets.getSprite(tile).draw({static_cast<float>(xPos),static_cast<float>(yPos)}, {(float) state.currentLevel.getTileSize(),(float) state.currentLevel.getTileSize()});
            // if (tile == 3) {tileColor= ORANGE;}

            // DrawRectangle(xPos, yPos, m_tileSize, m_tileSize, tileColor);
            // char msg1[64];
            // snprintf(msg1, sizeof(msg1), "%i, %i", j, i);
            // char msg2[64];
            // snprintf(msg2, sizeof(msg2), "%i, %i", xPos, yPos);
            // if (tile == 1) {
            //     DrawLineEx({(float) xPos, (float) yPos}, {(float) (xPos + m_tileSize),  (float) yPos}, 1, BLACK);
            //     DrawLineEx({(float) (xPos + m_tileSize), (float) yPos}, {(float) (xPos + m_tileSize), (float) (yPos + m_tileSize)}, 1, BLACK);
            //     DrawText(msg1, xPos, yPos, 5, BLACK);
            // }
            // DrawLineEx((Vector2) {xPos, yPos}, (Vector2) {xPos + tileSize, yPos}, 1, GRAY);
            // DrawLineEx((Vector2) {xPos, yPos}, (Vector2) {xPos, yPos + tileSize}, 1, GRAY);
            // DrawLineEx((Vector2) {xPos+tileSize, yPos}, (Vector2) {xPos + tileSize, yPos + tileSize}, 1, GRAY);
            // DrawLineEx((Vector2) {xPos, yPos+tileSize}, (Vector2) {xPos + tileSize, yPos+ tileSize}, 1, GRAY);
            // DrawText(msg1, xPos, yPos, 5, BLACK);
            // DrawText(msg2, xPos, yPos+10, 5, BLACK);
        }
    }

    
    Vector2 mousePos = GetMousePosition();
    if (state.mouseMode == 0 && mousePos.x - state.camera.offset.x >= 0 && mousePos.x - state.camera.offset.x < state.currentLevel.getWidth()*state.getTileSize() && mousePos.y - state.camera.offset.y >= 0 && mousePos.y - state.camera.offset.y < state.currentLevel.getHeight()*state.getTileSize()) { 
        // DrawRectangle(floor((mousePos.x-state.camera.offset.x)/state.getTileSize())*state.getTileSize(), floor((mousePos.y-state.camera.offset.y)/state.getTileSize())*state.getTileSize(), state.getTileSize(), state.getTileSize(), LIGHTGRAY);
        // assets.getSprite(state.selectedTile).draw({floor((mousePos.x-state.camera.offset.x)/state.getTileSize())*state.getTileSize(), floor((mousePos.y-state.camera.offset.y)/state.getTileSize())*state.getTileSize()}, {(float) state.getTileSize(),(float) state.getTileSize()});
        int offsetX = 0;
        int offsetY = 0;
        for(int i=0; i<state.selectedTiles.size(); i++) {
            Vector2 pos = {(floor((mousePos.x-state.camera.offset.x)/state.getTileSize()) + offsetX)*state.getTileSize(), (floor((mousePos.y-state.camera.offset.y)/state.getTileSize())+offsetY)*state.getTileSize()};
            assets.getSprite(state.selectedTiles[i]).draw(pos, {(float) state.getTileSize(),(float) state.getTileSize()});
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
    int levelCountBefore = levelRepository.getLevelCount();
    if (levelListSelection.selectedIndex >=0 && levelListSelection.selectedIndex < levelRepository.getLevelCount()) {
        levelRepository.deleteLevel(levelListSelection.selectedIndex);
    } else {
        std::cerr<<"ERROR [LevelEditor::deleteSelectedLevel] DELETE! index: "<<levelListSelection.selectedIndex<<std::endl;
        isOk = false;
    }
    
    syncCurrentLevelUi();

    int selected = levelListSelection.selectedIndex;
    
    if (selected >= levelListSelection.getTilesSize()) {
        selected = levelListSelection.getTilesSize() - 1;
    }
    
    levelListSelection.changeSelected(selected);

    if (isOk) {
        loadLevel(levelListSelection.selectedIndex);
    }
}

void LevelEditor::createNewLevel()
{
    for(int i=0; i<state.currentLevel.getLevelSize(); i++) {state.currentLevel.set(i, 0);};

    saveLevel("level");

    levelListSelection.changeSelected(levelListSelection.getTilesSize() - 1);
}

void LevelEditor::saveAllLevels()
{
    std::cout<<"savealll\n";
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
        DrawLineEx({(float)e.start.x*tileSize,(float) e.start.y*tileSize}, {(float)e.end.x*tileSize,(float) e.end.y*tileSize}, 4.0f, color);
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
    if (rect.height < 3 && rect.height >= 0) DrawLine(rect.x, rect.y, rect.x + rect.width, rect.y + rect.height, RED);

    DrawRectangleLinesEx(rectAbs(rect), thick, color);
}

void LevelEditor::drawSprites()
{

    int spriteSize = 32;
    Vector2 position = {0,300 + 210 + 10 + 60 + 80 + edgeAssignmentDropDown.getCount()*35 + state.mouseWheel*20};
    Vector2 startPosition = {0,300 + 210 + 10 + 60 + 80 + edgeAssignmentDropDown.getCount()*35};
    Rectangle selectionArea = {startPosition.x, startPosition.y, assets.getTextureWidth()*spriteSize, assets.getTextureHeight()*spriteSize};

    if (CheckCollisionPointRec(GetMousePosition(), selectionArea)) {
        state.mouseWheel += GetMouseWheelMove();
    }
    bool isSelected = false;
    Rectangle selected;
    bool isHover = false;
    Rectangle hover;
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
                // DrawRectangle(position.x-border, position.y-border, spriteSize+2*border, spriteSize+2*border, BLACK);
                selected = {position.x-border, position.y-border,(float) spriteSize+2*border,(float) spriteSize+2*border};
                isSelected = true;
            }
            if (CheckCollisionPointRec(GetMousePosition(), {position.x, position.y,(float) spriteSize,(float) spriteSize})) {
                int border = 3;
                // DrawRectangle(position.x-border, position.y-border, spriteSize+2*border, spriteSize+2*border, GRAY);
                hover = {position.x-border, position.y-border, (float) spriteSize+2*border,(float) spriteSize+2*border};
                isHover = true;
                hoverNum = i;

                if (IsMouseButtonDown(0)) {
                    state.selectedTile = i;
                }
            }
        
        sprite.draw(position, {(float) spriteSize,(float) spriteSize});

        }

        position.x += spriteSize;
        
    }

    if (isSelected)
        DrawRectangleLinesEx(selected, 2, BLACK);

    if (isHover) {
        DrawRectangleLinesEx(hover, 2, GRAY);
        // DrawText(std::to_string(hoverNum).c_str(), hover.x+10, hover.y, 32, BLUE);
    }

    if (CheckCollisionPointRec(GetMousePosition(), selectionArea)) {
        if (IsMouseButtonDown(0) && state.isSelection == false) {
            //first time;
            state.startSelection = GetMousePosition();
            state.isSelection = true;
        } 
        if (IsMouseButtonDown(0)) {
            //being pressed
            state.endSelection = GetMousePosition();
            state.selectedTiles.clear();
            Vector2 startP = {state.startSelection.x, state.startSelection.y - startPosition.y - state.mouseWheel*20};
            Vector2 endP= {state.endSelection.x, state.endSelection.y - startPosition.y - state.mouseWheel*20};
            // Rectangle absSelectedArea = rectAbs({state.startSelection.x, state.startSelection.y,state.endSelection.x - state.startSelection.x, state.endSelection.y - state.startSelection.y});
            Rectangle absSelectedArea = rectAbs({startP.x, startP.y, endP.x - startP.x, endP.y - startP.y});


            int startX = absSelectedArea.x/32;
            int startY = absSelectedArea.y/32;

            int endX= (absSelectedArea.x+absSelectedArea.width)/32;
            int endY= (absSelectedArea.y+absSelectedArea.height)/32;

            for(int y=startY; y<=endY; y++) {
                for(int x=startX; x<=endX; x++) {
                    state.selectedTiles.push_back(y*assets.getTextureWidth() + x);
                }
            }

        } else {
            state.isSelection = false;
        }



        Rectangle linesRect = {state.startSelection.x, state.startSelection.y,state.endSelection.x - state.startSelection.x, state.endSelection.y - state.startSelection.y};
        // DrawRectangleLinesEx({state.startSelection.x, state.startSelection.y,state.endSelection.x - state.startSelection.x, state.endSelection.y - state.startSelection.y}, 3 , RED);
        // drawRectLines(linesRect, 3, RED);
        // DrawRectangle(state.startSelection.x, state.startSelection.y, 10, 10, RED); 
        // DrawRectangle(startPosition.x, startPosition.y, 10, 10, RED);

    } else {
        state.isSelection = false;
    }

    for(int i=0; i<state.selectedTiles.size(); i++) {
        Rectangle rect = {(state.selectedTiles[i] % assets.getTextureWidth()) * spriteSize, (state.selectedTiles[i] / assets.getTextureWidth()) * spriteSize, spriteSize, spriteSize};
        if (rect.y + startPosition.y + state.mouseWheel*20 >= selectionArea.y )
            DrawRectangleLinesEx({rect.x + startPosition.x, rect.y + startPosition.y + state.mouseWheel*20, rect.width, rect.height}, 2, GRAY);
    }

    DrawRectangleLines(selectionArea.x, selectionArea.y, selectionArea.width, selectionArea.height, BLACK);
    
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
