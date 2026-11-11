#pragma once

#include "raylib.h"
#include <string>

struct SaveData {
    int levelIndex = 11;

    Vector2 playerPosition = {0.0f, 0.0f};
    Vector2 playerVelocity = {0.0f, 0.0f};
    bool facingRight = true;
    bool onGround = false;

    int jumpCount = 0;
    float elapsedTime = 0.0f;
};

bool saveGame(const SaveData& data, const std::string& filename = "save.txt");
bool loadGame(SaveData& data, const std::string& filename = "save.txt");
bool hasSaveGame(const std::string& filename = "save.txt");