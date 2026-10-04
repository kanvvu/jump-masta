#pragma once

#include "raylib.h"
#include "interfaces.hpp"
#include "assets.hpp"

class Player : IDraw, IUpdate
{
private:
    int m_tileSize;

public:
    Vector2 m_position;
    Vector2 m_previousPosition;
    Vector2 m_velocity;
    Vector2 m_direction;
    bool onGround = false;
    Sprite sprite;


    Player(int tileSize, Vector2 position, Sprite sprite);
    ~Player() = default;

    void draw();
    void update(float elapsedTime);

    void setTileSize(int tileSize) {m_tileSize = tileSize;}

    void reset();
};

