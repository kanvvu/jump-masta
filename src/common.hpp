#pragma once

#include <vector>
#include <string>

class LevelRepository;
class Assets;

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

struct CollisionPoint {
    Vector2i point;
    EdgeType side;
};

struct Edge {
    Vector2i start;
    Vector2i end;
    int assignedNextLevel; 
    int tilesCount;
    EdgeType type;
};

struct DropDownState {
    int dropdownActive = -1;
    bool dropDownEditMode = false;
};