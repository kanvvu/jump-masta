#include "player.hpp"
#include "raymath.h"
#include <iostream>
#include <algorithm>
#include <string>

Player::Player() :
    m_tileSize(32),
    m_position({0, 0}),
    m_previousPosition({0,0}),
    m_velocity({0, 0}),
    m_direction({0, 0})
{
}

Player::Player(int tileSize, Vector2 position) :
    m_tileSize(tileSize),
    m_position(position),
    m_previousPosition(position),
    m_velocity({0, 0}),
    m_direction({0, 0})
{
}

void Player::draw()
{
    DrawRectangle(m_position.x*m_tileSize, m_position.y*m_tileSize, m_tileSize, m_tileSize, MAROON);
    std::string msg = "" +  std::to_string(m_velocity.x) + ", " + std::to_string(m_velocity.y);
    DrawText(msg.c_str(), m_position.x*m_tileSize, m_position.y*m_tileSize-20, 18, BLACK);

    msg = "" +  std::to_string(m_position.x) + ", " + std::to_string(m_position.y);
    DrawText(msg.c_str(), m_position.x*m_tileSize, m_position.y*m_tileSize-40, 18, BLACK);
}

void Player::update(float elapsedTime)
{
    Vector2 accel = m_direction;

    m_velocity = Vector2Add(m_velocity, accel);


    float velocityThreshold = 20.0;
    if (m_velocity.x > velocityThreshold) m_velocity.x = velocityThreshold;
    if (m_velocity.x < -velocityThreshold) m_velocity.x = -velocityThreshold;

    if (m_velocity.y > velocityThreshold) m_velocity.y = velocityThreshold;
    if (m_velocity.y < -velocityThreshold) m_velocity.y = -velocityThreshold;

    float friction = 0.95;
    m_velocity = Vector2Scale(m_velocity, friction);
    if (fabs(Vector2Distance({0,0}, m_velocity)) < 0.1) m_velocity = {0,0};


    Vector2 halfAccel = Vector2Scale(accel, 0.5);
    Vector2 velocityAndAccel = Vector2Add(m_velocity, halfAccel);
    Vector2 move = Vector2Scale(velocityAndAccel, elapsedTime); 
    Vector2 new_position = Vector2Add(m_position, move); 

    m_previousPosition = m_position;
    m_position = new_position;

}

void Player::reset()
{
    m_direction = Vector2Zero();
    m_position = Vector2Zero();
    m_previousPosition = Vector2Zero();
    m_velocity = Vector2Zero();

}
