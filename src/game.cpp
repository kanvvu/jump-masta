#include "game.hpp"
#include "raymath.h"
#include <iostream>
#include <string>

Game::Game(int windowWidth, int windowHeight, const char *windowTitle)
  : m_windowWidth(windowWidth),
    m_windowHeight(windowHeight),
    m_windowTitle(windowTitle),
    m_tileMap(10, 15)
{
    InitWindow(windowWidth, windowHeight, windowTitle);
    SetTargetFPS(60);

    m_player = Player(m_tileMap.getTileSize(), m_tileMap.getSpawnPosition());
}

Game::~Game()
{
    CloseWindow();
}

void Game::start()
{

    while (!WindowShouldClose())
    {

        proccessInput();
        update();
        draw();
    }
}

void Game::proccessInput()
{
    Vector2 dir = {0.0, 0.0};
    if (IsKeyDown(KEY_W)) { dir.y = -1;}
    if (IsKeyDown(KEY_A)) { dir.x = -1;}
    if (IsKeyDown(KEY_S)) { dir.y = 1;}
    if (IsKeyDown(KEY_D)) { dir.x = 1;}

    if (IsKeyPressed(KEY_R)) {
        m_player.reset();
        m_player.m_position = m_tileMap.getSpawnPosition();
    }

    m_player.m_direction = Vector2Normalize(dir);

}

void Game::update()
{
    m_player.update(GetFrameTime());

    Vector2 bP = m_player.m_previousPosition; 
    Vector2 nBP = m_player.m_position;
    
    // velo = Vector2Subtract(nBP, bP);
    std::string msg = "";
    Vector2 movementDirection = Vector2Subtract(nBP, bP);
    if (movementDirection.x <= 0) { //left
        if (m_tileMap.isCollision({nBP.x, bP.y}, 1) ||
            m_tileMap.isCollision({nBP.x, bP.y + 0.99f}, 1)) {
            nBP.x = (int) nBP.x + 1;
            m_player.m_velocity.x = -0.5*m_player.m_velocity.x;
            // std::cout<<"left\n";
            msg += "| left (" + std::to_string(nBP.x) + ", " + std::to_string(nBP.y) + "), " + "(" + std::to_string(m_player.m_velocity.x) + ", " + std::to_string(m_player.m_velocity.y) + ") ";
            }
    }
    else {  //right
        if (m_tileMap.isCollision({nBP.x + 1.0f, bP.y}, 1) ||
            m_tileMap.isCollision({nBP.x + 1.0f, bP.y + 0.99f}, 1)) {
            nBP.x = (int) nBP.x;
            m_player.m_velocity.x = -0.5*m_player.m_velocity.x;
            // std::cout<<"right\n";
            msg += "| right ";
        }
    }

    // onGround = false;
    if (movementDirection.y <= 0) { //up
        if (m_tileMap.isCollision({nBP.x, nBP.y}, 1) ||
            m_tileMap.isCollision({nBP.x + 0.90f, nBP.y}, 1)) {
            nBP.y = (int) nBP.y + 1;
            m_player.m_velocity.y = -0.5*m_player.m_velocity.y;
            // std::cout<<"up " <<"("<<nBP.x << ", " << nBP.y <<"), "<< "("<<m_player.m_velocity.x << ", " << m_player.m_velocity.y <<"), " << "\n";
            msg += "| up (" + std::to_string(nBP.x) + ", " + std::to_string(nBP.y) + "), " + "(" + std::to_string(m_player.m_velocity.x) + ", " + std::to_string(m_player.m_velocity.y) + ") ";
        }
    }
    else { //down
        if (m_tileMap.isCollision({nBP.x, nBP.y + 1.0f}, 1) ||
            m_tileMap.isCollision({nBP.x + 0.90f, nBP.y + 1.0f}, 1)) {
            nBP.y = (int) nBP.y;
            m_player.m_velocity.y = 0;
            // std::cout<<"down\n";
            msg += "| down ";
            // onGround = true;
        }
    }
    if (msg != "") std::cout<<msg<<"\n";

    m_player.m_position = nBP;



}

void Game::draw()
{
    BeginDrawing();
    ClearBackground(DARKGRAY);

    m_tileMap.draw();
    m_player.draw();

    EndDrawing();
}