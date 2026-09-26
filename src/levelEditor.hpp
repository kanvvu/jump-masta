#pragma once

#include "raylib.h"
#include "interfaces.hpp"
#include "common.hpp"
#include <vector>
#include "levelRepository.hpp"
#include "levelData.hpp"
#include "levelEditorGui.hpp"


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

    int tileSize = DefaultTileSize;
    bool isMouseDown = false;
    int mouseMode = 0; // 0 - adding, 1 - removing

    int selectedTile = 1;
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

    void saveCurrentLevel();
    void loadSelectedLevel();
    void deleteSelectedLevel();
    void createNewLevel();
    void saveAllLevels();
    void applyLevelDimensionsFromUi();


public:
    LevelEditor(LevelRepository& levelRepository);
    LevelEditor(LevelRepository& levelRepository, int tileSize);
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

    void syncCurrentLevelUi();
    std::vector<std::string> getLevelNames();

    void saveLevel(std::string name);
    void loadLevel(int levelID);

};

