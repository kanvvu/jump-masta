#pragma once

#include "raylib.h"
#include "app/interfaces.hpp"
#include "assets/assets.hpp"

namespace PlayerConfig {
    constexpr float GRAVITY = 45.0f;                // tiles/s^2
    constexpr float GROUND_FRICTION = 0.50f;        // dacay base 1/60s
    constexpr float STOP_VELOCITY_THRESHOLD = 1.0f; // tiles/s
}

enum class PlayerState {
    Idle,
    Jumping,
    Falling,
    Aiming
};

class Player : IUpdate
{
    
    public:
    Vector2 m_position;
    Vector2 m_previousPosition;
    Vector2 m_velocity;
    Vector2 m_direction;
    PlayerAnimations m_playerAnimations;
    bool onGround = false;
    bool m_facingRight = true;
    bool m_hitWall = false;
    bool m_isAiming = false;

    PlayerState m_previousState;
    
    Player(const PlayerAnimations& playerAnimations);
    Player(Vector2 position, const PlayerAnimations& playerAnimations);
    ~Player() = default;

    PlayerState getState() const;
    
    void draw(int tileSize);
    void update(float elapsedTime);
    
    void reset(Vector2 spawnPos = {0.0f, 0.0f});

    Animation& getActiveAnimation();
};

