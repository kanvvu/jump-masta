#pragma once

#include "raylib.h"
#include "interfaces.hpp"
#include "common.hpp"
#include <vector>


class LevelEditor
{
private:
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

public:
    LevelEditor();
    LevelEditor(int tileSize);
    ~LevelEditor() = default;

    void proccessInput(GameState& gameState);
    void update(float deltaTime);
    void draw();
};

