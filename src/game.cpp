#include "game.hpp"

Game::Game(int windowWidth, int windowHeight, const char *windowTitle)
  : m_windowWidth(windowWidth),
    m_windowHeight(windowHeight),
    m_windowTitle(windowTitle),
    m_tileMap(10, 15)
{
    InitWindow(windowWidth, windowHeight, windowTitle);
    SetTargetFPS(60);
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
    //to do
}

void Game::update()
{
    //to do
}

void Game::draw()
{
    BeginDrawing();
    ClearBackground(DARKGRAY);

    m_tileMap.draw();


    
    DrawText("HELLO!", 0, 0, 20, WHITE);
    EndDrawing();
}