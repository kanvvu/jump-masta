#include "player.hpp"
#include "raymath.h"
#include <iostream>
#include <algorithm>
#include <string>


// Player::Player() :
//     m_tileSize(32),
//     m_position({0, 0}),
//     m_previousPosition({0,0}),
//     m_velocity({0, 0}),
//     m_direction({0, 0})
// {
// }

Player::Player(Sprite sprite) : Player({0.0f, 0.0f}, sprite)
{
}

Player::Player(Vector2 position, Sprite sprite) : m_position(position),
                                                  sprite(sprite),
                                                  m_previousPosition(position),
                                                  m_velocity({0, 0}),
                                                  m_direction({0, 0})

{
}

void Player::draw(int tileSize)
{
    // DrawRectangle(m_position.x*m_tileSize, m_position.y*m_tileSize, m_tileSize, m_tileSize, MAROON);
    sprite.draw({m_position.x*tileSize, m_position.y*tileSize}, {64,64});
    std::string msg = "" +  std::to_string(m_velocity.x) + ", " + std::to_string(m_velocity.y);
    DrawText(msg.c_str(), m_position.x*tileSize, m_position.y*tileSize-20, 18, BLACK);

    msg = "" +  std::to_string(m_position.x) + ", " + std::to_string(m_position.y);
    DrawText(msg.c_str(), m_position.x*tileSize, m_position.y*tileSize-40, 18, BLACK);
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
}

PlayerState Player::getState() const
{
    if (onGround)
        return PlayerState::Idle;
    else if (m_velocity.y < 0.0f)
        return PlayerState::Jumping;
    else
        return PlayerState::Falling;
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
}
