#pragma once

#include <vector>
#include <string>
#include "raylib.h"

class LevelRepository;
class Assets;

enum class GameState {
    GAME,
    LEVEL_EDITOR
};

struct Vector2i {
    int x;
    int y;
};

enum class EdgeType {
    TOP,
    DOWN,
    LEFT,
    RIGHT
};

enum class CellType {
    Empty,
    Solid,
    Spawn
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

inline Vector2 calculateCameraCenterOffset(Vector2i windowSize, Vector2i mapTileCount, int tileSize) {
    return {
        windowSize.x / 2.0f - (mapTileCount.x * tileSize) / 2.0f,
        windowSize.y / 2.0f - (mapTileCount.y * tileSize) / 2.0f
    };
}