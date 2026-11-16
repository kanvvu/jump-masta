#pragma once

#include <vector>
#include <string>
#include "raylib.h"
#include <tuple>

class LevelRepository;
class Assets;

struct GameSettings {
    int showFps = false;
    int showTimer= false;
    int showJumpCount = false;
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
    Spawn,
    Goal,
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

namespace CellTypeInfo {
    inline constexpr int collisonIDs[] = {182, 183, 184,
                                   204, 205, 206,
                                   226, 227, 228, 229, 230, 231,
                                   188, 189, 190, 191,
                                   210, 211, 212, 213,
                                   233, 234, 235,
                                   94, 95, 96, 97, 98,
                                   116, 117, 118, 119, 120,
                                   138, 139, 140,
                                   281, 282, 283, 284,
                                   303, 304, 305, 306,
                                   326, 327, 328,
                                   105, 106, 107};
    inline CellType getCellType(int spriteId)
    {

        
        if (spriteId == 47) return CellType::Spawn;
        if (spriteId == 48) return CellType::Goal;
        
        
        for (const auto& collsionID : collisonIDs) {
            if (spriteId == collsionID) {
                return CellType::Solid;
            }
        } 

        return CellType::Empty;
    }
}