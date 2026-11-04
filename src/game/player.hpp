#pragma once

#include "raylib.h"
#include "app/interfaces.hpp"
#include "assets/assets.hpp"

class Player : IUpdate
{
public:
    Vector2 m_position;
    Vector2 m_previousPosition;
    Vector2 m_velocity;
    Vector2 m_direction;
    bool onGround = false;
    Sprite sprite;

    Player(Vector2 position, Sprite sprite);
    ~Player() = default;

    void draw(int tileSize);
    void update(float elapsedTime);

    void reset();
};

