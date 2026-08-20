#include "line.hpp"

#include "raymath.h"

void Line::update(float elapsedTime)
{
    if (isLeftMouseButtonPressed){
        float lineLength = Vector2Length(Vector2Subtract(GetMousePosition(), startLinePos)); 
        //printf("%f\n", lineLength);

        if (lineLength < 300.0) lineColor = RED;
        if (lineLength < 200.0) lineColor = ORANGE;
        if (lineLength < 100.0) lineColor = BLACK;
    }
}

void Line::draw(Vector2 offset)
{
    if (isLeftMouseButtonPressed) {
        // Vector2 newStartLinePos = Vector2Subtract(Vector2Scale(m_player.m_position, 64), startLinePos);
        // DrawLineEx(Vector2Scale(m_player.m_position, 64), Vector2Add(GetMousePosition(), newStartLinePos), 3, lineColor);
        DrawLineEx(startLinePos, GetMousePosition(), 3, lineColor);
    }

}

void Line::mouseButtonDown()
{
    if (!isLeftMouseButtonPressed) {
        isLeftMouseButtonPressed = true;
        startLinePos = GetMousePosition();
    }
}

void Line::mouseButtonUp(Vector2& velocity)
{
    if (isLeftMouseButtonPressed) {
        velocity = Vector2Subtract(GetMousePosition(), startLinePos);
        velocity = Vector2Scale(velocity, 0.07);
        // printf("realsed! (%f, %f)\n", velo.x, velo.y);
    }
    isLeftMouseButtonPressed = false;
}
