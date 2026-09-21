#pragma once

#include "raylib.h"
#include "interfaces.hpp"
#include "common.hpp"
#include <vector>
#include "levelRepository.hpp"
#include "levelData.hpp"
#include "levelEditorGui.hpp"


class LevelEditor
{
private:


    LevelEditorGui gui;

    int m_tileSize = 64;

    Camera2D camera = {0};
    Vector2 cameraOffset = {0};

    LevelData currLevel;

    int screenWidth = 1920;
    int screenHeight = 1080;

    int tileSize = 64;
    bool isMouseDown = false;
    int mouseMode = 0; // 0 - adding, 1 - removing

    int selectedTile = 1;

    LevelRepository& levelRepository;


public:
    LevelEditor(LevelRepository& levelRepository);
    LevelEditor(LevelRepository& levelRepository, int tileSize);
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

