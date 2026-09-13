#pragma once

#include <vector>
#include <string>

enum GameState {
    GAME,
    LEVEL_EDITOR
};

struct Vector2i {
    int x;
    int y;
};

enum EdgeType {
    TOP,
    DOWN,
    LEFT,
    RIGHT
};

struct Edge {
    Vector2i start;
    Vector2i end;
    int assignedNextLevel; 
    int tilesCount;
    EdgeType type;
};

struct LevelData {
    std::vector<int> level;
    std::vector<Edge> edges;

    int width;
    int height;
    int tileSize;
    
    std::string name;
};