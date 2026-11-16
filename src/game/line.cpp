#include "line.hpp"

#include "raymath.h"

void Line::update([[maybe_unused]] float elapsedTime)
{
    if (isLeftMouseButtonPressed){
        float dragDistance = Vector2Length(Vector2Subtract(GetMousePosition(), startLinePos)); 
        float dragRatio = Clamp(dragDistance/LineConfig::MAX_DRAG_DISTANCE, 0.0, 1.0f) ;
        
        lineColor = BLACK;
        if (dragRatio > 0.5) lineColor = ORANGE;
        if (dragRatio > 0.9f) lineColor = RED;
    }
}

void Line::draw()
{
    if (isLeftMouseButtonPressed) {
        Vector2 dragVector = Vector2Subtract(GetMousePosition(), startLinePos);
        dragVector = Vector2ClampValue(dragVector, 0.0f, LineConfig::MAX_DRAG_DISTANCE); 
        DrawLineEx(startLinePos, Vector2Add(startLinePos,dragVector), 3, lineColor);
    }

}

void Line::mouseButtonDown()
{
    if (!isLeftMouseButtonPressed) {
        isLeftMouseButtonPressed = true;
        startLinePos = GetMousePosition();
    }
}

std::optional<Vector2> Line::mouseButtonUp()
{
    std::optional<Vector2> launchImpulse;
    if (isLeftMouseButtonPressed) {
        Vector2 dragVector = Vector2Subtract(GetMousePosition(), startLinePos);
        float dragDistance = Vector2Length(dragVector);

        if (dragDistance >= LineConfig::MIN_DRAG_DISTANCE) {
            float powerRatio = Clamp(dragDistance/LineConfig::MAX_DRAG_DISTANCE, 0.0f, 1.0f);
            dragVector = Vector2Scale(Vector2Normalize(dragVector), powerRatio*LineConfig::MAX_LAUNCH_SPEED);
            launchImpulse = std::make_optional(dragVector);

        }

    }
    isLeftMouseButtonPressed = false;
    return launchImpulse;
}
