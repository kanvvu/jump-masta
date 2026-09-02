#pragma once

#include "raylib.h"
#include "interfaces.hpp"
#include "common.hpp"



class LevelEditor
{
private:
    int m_tileSize = 64;

    Camera2D camera = {0};
    Vector2 cameraOffset = {0};

public:
    LevelEditor();
    LevelEditor(int tileSize);
    ~LevelEditor() = default;

    void proccessInput(GameState& gameState);
    void update(float deltaTime);
    void draw();
};

