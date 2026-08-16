#include "game.hpp"

Game::Game(int windowWidth, int windowHeight, const char *windowTitle)
  : m_windowWidth(windowWidth),
    m_windowHeight(windowHeight),
    m_windowTitle(windowTitle),
    m_tileMap(10, 15)
{
    InitWindow(windowWidth, windowHeight, windowTitle);
    SetTargetFPS(60);

    m_player = Player(m_tileMap.getTileSize(), m_tileMap.getPlayerPosition());
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

    m_player.move(dir);

}

void Game::update()
{
    m_player.update(GetFrameTime());
}

void Game::draw()
{
    BeginDrawing();
    ClearBackground(DARKGRAY);

    m_tileMap.draw();
    m_player.draw();

    EndDrawing();
}