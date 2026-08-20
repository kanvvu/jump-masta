#pragma once

#include "raylib.h"
#include "interfaces.hpp"
#include "player.hpp"


class Line : IDraw, IUpdate
{
private:

    Color lineColor = BLACK;
    bool isLeftMouseButtonPressed = false;
    Vector2 startLinePos = {0,0};

public:
    Line() = default;
    ~Line() = default; 


    void update(float elapsedTime);
    void draw(Vector2 offset);

    void mouseButtonDown();
    void mouseButtonUp(Vector2& velocity);
};

