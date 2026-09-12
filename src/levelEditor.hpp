#pragma once

#include "raylib.h"
#include "interfaces.hpp"
#include "common.hpp"
#include <vector>
#include "tileLevelManager.hpp"


class LevelEditor
{
private:
    struct DropDownState {
        int dropdownActive = -1;
        bool dropDownEditMode = false;
    };

    struct EditorGuiState
    {
        std::vector<std::string> tileNames;
        std::vector<char *> tileList;

        int tileListScroll = 0;
        int tileListSelected = -1;
        int tileListHovered = -1;

        void changeSelected(int index, int maxScroll = 6);

        void rebuildTileList();
        
        std::string dropDownOptions;
        std::vector<DropDownState> dropDowns;
        
        void rebuildDropDowns();

        int saveButton = 0;
        int loadButton = 0;
    };

    EditorGuiState gui;

    int m_tileSize = 64;

    Camera2D camera = {0};
    Vector2 cameraOffset = {0};
    std::vector<int> currMap;

    int screenWidth = 1920;
    int screenHeight = 1080;

    int tileCountX = 10;
    int tileCountY = 15;
    int tileSize = 64;
    bool isMouseDown = false;
    int mouseMode = 0; // 0 - adding, 1 - removing

    int selectedTile = 1;

    TileLevelManager& tileLevelManager;

    std::vector<Edge> edges;

public:
    LevelEditor(TileLevelManager& tileLevelManager);
    LevelEditor(TileLevelManager& TileLevelManager, int tileSize);
    ~LevelEditor() = default;

    void proccessInput(GameState& gameState);
    void update(float deltaTime);
    void draw();
    void drawGUI();
    void drawEdges();

    void createTileNames();

    void saveLevel(std::string name);
    void loadLevel(int levelID);

    void buildEdges();
};

