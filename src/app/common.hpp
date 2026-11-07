#pragma once

#include <vector>
#include <string>
#include "raylib.h"
#include <tuple>

class LevelRepository;
class Assets;

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

using LevelId = int;

struct ExitKey {
    EdgeType side;
    int start;

    bool operator<(const ExitKey& other) const {
        return std::tie(side, start) < std::tie(other.side, other.start);
    }

    bool operator==(const ExitKey& other) const {
        return side == other.side && start == other.start;
    }

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

    ExitKey getKey() const {
        int offset = (type == EdgeType::TOP || type == EdgeType::DOWN) ? start.x : start.y;
        return ExitKey{type, offset};
    }
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