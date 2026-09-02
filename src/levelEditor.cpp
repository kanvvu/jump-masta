#include "levelEditor.hpp"

#include "raylib.h"

void LevelEditor::proccessInput(GameState& gameState)
{
    if (IsKeyPressed(KEY_E)) {
        gameState = GAME;
    }   
}

void LevelEditor::update()
{
}

void LevelEditor::draw()
{
    ClearBackground(BLACK);
}