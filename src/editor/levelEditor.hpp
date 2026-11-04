#pragma once

#include "raylib.h"
#include "app/interfaces.hpp"
#include "app/common.hpp"
#include <vector>
#include "level/levelRepository.hpp"
#include "level/levelData.hpp"
#include "levelEditorGui.hpp"
#include "assets/assets.hpp"


struct LevelEditorState
{
    static constexpr int DefaultLevelWidth = 10;
    static constexpr int DefaultLevelHeight = 15;
    static constexpr int DefaultScreenWidth = 1920;
    static constexpr int DefaultScreenHeight = 1080;
    static constexpr int DefaultTileSize = 64;

    LevelData currentLevel{DefaultLevelWidth, DefaultLevelHeight};
    Camera2D camera = {0};
    Vector2 cameraOffset = {0};

    int screenWidth = DefaultScreenWidth;
    int screenHeight = DefaultScreenHeight;

    bool isMouseDown = false;
    int mouseMode = 0; // 0 - adding, 1 - removing

    int selectedTile = 1;
    std::vector<int> selectedTiles = {1};
    int getTileSize() {return currentLevel.getTileSize();}

    int mouseWheel = 0;

    Vector2 startSelection;
    Vector2 endSelection;
    bool isSelection = false;

};

enum class EditorCommand
{
    SaveCurrentLevel,
    SaveAllLevels,
    LoadSelectedLevel,
    DeleteSelectedLevel,
    CreateNewLevel,
    ApplyLevelDimensions,
};

class LevelEditor
{
private:

    SmallTextBox nameTextBox;
    SmallTextBox widthTextBox;
    SmallTextBox heightTextBox;
    SmallTextBox tileSizeTextBox;
    Button addNewButton;
    Button saveAllButton;
    ListView levelListSelection;
    Button deleteButton;
    Button saveButton;
    Button saveAsNewButton;
    TextLabelAndDropDownBoxList edgeAssignmentDropDown;

    std::vector<IDraw*> guiItems;

    LevelEditorState state;

    LevelRepository& levelRepository;
    Assets& assets;

    void saveCurrentLevel();
    void loadSelectedLevel();
    void deleteSelectedLevel();
    void createNewLevel();
    void saveAllLevels();
    void applyLevelDimensionsFromUi();


public:
    LevelEditor(LevelRepository& levelRepository, Assets& assets);
    LevelEditor(LevelRepository& levelRepository, Assets& assets, int tileSize);
    ~LevelEditor() = default;

    void proccessInput(GameState& gameState);
    void update(float deltaTime);
    void draw();
    void drawGUI();
    void handleLevelNameInput();
    void handleLevelDimensionsInput();
    void handleAddNewLevel();
    void handleSaveAll();
    void handleLevelListSelection();
    void handleLevelDelete();
    void handleLevelSave();
    void handleSaveAsNew();
    void drawEdges();

    void drawSprites();

    void syncCurrentLevelUi();
    std::vector<std::string> getLevelNames();

    void saveLevel(std::string name);
    void loadLevel(int levelID);

};

