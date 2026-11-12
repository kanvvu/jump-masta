#pragma once

#include "raylib.h"
#include "app/interfaces.hpp"
#include <optional>


namespace LineConfig {
    constexpr float MAX_DRAG_DISTANCE = 300.0f;
    constexpr float MIN_DRAG_DISTANCE = 15.0f;
    constexpr float MAX_LAUNCH_SPEED = 40.0f;
}

class Line : IDraw, IUpdate
{
private:

    Color lineColor = BLACK;
    bool isLeftMouseButtonPressed = false;
    Vector2 startLinePos = {0,0};

    float getLength();
    float getClampedLength();

public:
    Line() = default;
    ~Line() = default; 


    void update(float elapsedTime);
    void draw();
    
    void reset() {isLeftMouseButtonPressed = false; startLinePos = {0,0}; }

    void mouseButtonDown();
    std::optional<Vector2> mouseButtonUp();
};

