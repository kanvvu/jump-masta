#pragma once

#include "raylib.h"
#include "interfaces.hpp"

class Player : IDraw, IUpdate
{
private:
    int m_tileSize;

public:
    Vector2 m_position;
    Vector2 m_previousPosition;
    Vector2 m_velocity;
    Vector2 m_direction;

    Player();
    Player(int tileSize, Vector2 position);
    ~Player() = default;

    void draw();
    void update(float elapsedTime);
};

