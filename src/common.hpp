#pragma once

#include <vector>
#include <string>

enum GameState {
    GAME,
    LEVEL_EDITOR
};

struct TileMapLevel {
    std::vector<int> level;

    int width;
    int height;
    
    std::string name;
};