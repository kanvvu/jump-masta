#include "game.hpp"
#include "raymath.h"

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

    m_player.m_direction = Vector2Normalize(dir);

}

void Game::update()
{
    m_player.update(GetFrameTime());

    Vector2 bP = m_player.m_previousPosition; 
    Vector2 nBP = m_player.m_position;
    
    // velo = Vector2Subtract(nBP, bP);

    if (m_player.m_velocity.x <= 0) { //left
        if (m_tileMap.isCollision({nBP.x, bP.y}, 1) ||
            m_tileMap.isCollision({nBP.x, bP.y + 0.99f}, 1)) {
            nBP.x = (int) nBP.x + 1;
            m_player.m_velocity.x = -0.5*m_player.m_velocity.x;
            }
    }
    else {  //right
        if (m_tileMap.isCollision({nBP.x + 1.0f, bP.y}, 1) ||
            m_tileMap.isCollision({nBP.x + 1.0f, bP.y + 0.99f}, 1)) {
            nBP.x = (int) nBP.x;
            m_player.m_velocity.x = -0.5*m_player.m_velocity.x;
        }
    }

    // onGround = false;
    if (m_player.m_velocity.y <= 0) { //up
        if (m_tileMap.isCollision({nBP.x, nBP.y}, 1) ||
            m_tileMap.isCollision({nBP.x + 0.90f, nBP.y}, 1)) {
            nBP.y = (int) nBP.y + 1;
            m_player.m_velocity.y = -0.5*m_player.m_velocity.y;
        }
    }
    else { //down
        if (m_tileMap.isCollision({nBP.x, nBP.y + 1.0f}, 1) ||
            m_tileMap.isCollision({nBP.x + 0.90f, nBP.y + 1.0f}, 1)) {
            nBP.y = (int) nBP.y;
            m_player.m_velocity.y = 0;
            // onGround = true;
        }
    }

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