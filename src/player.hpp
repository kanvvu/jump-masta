#pragma once

#include "raylib.h"
#include "interfaces.hpp"

class Player : IDraw, IUpdate
{
private:
    Vector2 m_position;
    Vector2 m_velocity;
    Vector2 m_direction;

    int m_tileSize;

public:
    Player();
    Player(int tileSize, Vector2 position);
    ~Player() = default;

    void draw();
    void update(float elapsedTime);

    void move(Vector2 dir);
};

