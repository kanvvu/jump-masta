#include "player.hpp"
#include "raymath.h"
#include <iostream>
#include <algorithm>
#include <string>

Player::Player(const PlayerAnimations& playerAnimations) 
: Player({0.0f, 0.0f}, playerAnimations)
{
}

Player::Player(Vector2 position,const PlayerAnimations& playerAnimations)
 :  m_position(position),
    m_previousPosition(position),
    m_velocity({0, 0}),
    m_direction({0, 0}),
    m_playerAnimations(playerAnimations)
{
    m_previousState = getState();
}

void Player::draw(int tileSize)
{
    getActiveAnimation().getCurrentSprite().draw({m_position.x*tileSize, m_position.y*tileSize}, {64,64}, !m_facingRight);
}

void Player::update(float elapsedTime)
{
    m_hitWall = false;

    Vector2 accel = {0, PlayerConfig::GRAVITY};

    m_velocity = Vector2Add(m_velocity, accel*elapsedTime);

    if (onGround) {
        float frictionFactor = powf(PlayerConfig::GROUND_FRICTION, elapsedTime*60.0f);
        m_velocity.x *= frictionFactor;

        if (Vector2Length(m_velocity) < PlayerConfig::STOP_VELOCITY_THRESHOLD) 
            m_velocity.x = 0.0f;
    }


    Vector2 move = Vector2Scale(m_velocity, elapsedTime); 
    Vector2 new_position = Vector2Add(m_position, move); 

    m_previousPosition = m_position;
    m_position = new_position;

    if (m_velocity.x > 0.1f) m_facingRight = true;
    if (m_velocity.x < -0.1f) m_facingRight = false;

    if(m_previousState != getState()) {
        getActiveAnimation().reset();
    }

    getActiveAnimation().update(elapsedTime);

    m_previousState = getState();
}

PlayerState Player::getState() const
{
    if (!onGround) {
        if (m_velocity.y < 0.0f)
            return PlayerState::Jumping;
        return PlayerState::Falling;
    }

    if (m_isAiming)
        return PlayerState::Aiming;
    
    return PlayerState::Idle;
}

void Player::reset(Vector2 spawnPos)
{
    m_direction = Vector2Zero();
    m_position = spawnPos;
    m_previousPosition = spawnPos;
    m_velocity = Vector2Zero();
    m_facingRight = true;
    m_hitWall = false;
    onGround = false;
    m_isAiming = false;
    m_previousState = getState();
}

Animation &Player::getActiveAnimation()
{
    switch (getState())
    {
    case PlayerState::Aiming:
        return m_playerAnimations.aiming; 
    case PlayerState::Falling:
        return m_playerAnimations.falling; 
    case PlayerState::Idle:
        return m_playerAnimations.idle; 
    case PlayerState::Jumping:
        return m_playerAnimations.jumping; 
    }

    return m_playerAnimations.idle;
}
