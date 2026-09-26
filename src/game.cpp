#include "game.hpp"
#include "raymath.h"
#include <iostream>
#include <string>

Game::Game(int windowWidth, int windowHeight, const char *windowTitle)
  : m_raylibContext(windowWidth, windowHeight, windowTitle),
    m_windowWidth(windowWidth),
    m_windowHeight(windowHeight),
    m_windowTitle(windowTitle),
    m_levelRepository(m_levelFileLoader),
    m_tileMap(m_levelRepository),
    m_levelEditor(m_levelRepository)
{
    // InitWindow(windowWidth, windowHeight, windowTitle);
    SetTargetFPS(60);

    camera.offset.x = windowWidth/2 - (m_tileMap.getSize().x*m_tileMap.getTileSize())/2;   // coordinate x origin
    camera.offset.y = windowHeight/2 - (m_tileMap.getSize().y*m_tileMap.getTileSize())/2;   // coordinate y origin
    camera.target.x = 0;         // rotation and zoom x origin (from offset above)
    camera.target.y = 0;         // rotation and zoom y origin (from offset above)
    camera.rotation = 0;        // rotation in deg
    camera.zoom     = 1.0f;      // magnification, i.e fov or zoom

    m_player = Player(m_tileMap.getTileSize(), m_tileMap.getSpawnPosition());
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

void Game::proccessInputGame() {
    if (IsKeyPressed(KEY_E)) {
        currentGameState = LEVEL_EDITOR;
    }   

    if (IsKeyPressed(KEY_R)) {
        m_player.reset();
        m_tileMap.changeLevel(0);
        m_player.m_position = m_tileMap.getSpawnPosition();
    }

    if (IsMouseButtonDown(0)) {
        m_line.mouseButtonDown();
    }

    if (!IsMouseButtonDown(0)) {
        m_line.mouseButtonUp(m_player.m_velocity);
    }
}

void Game::proccessInput()
{

    switch (currentGameState)
    {
    case GAME:
        proccessInputGame();
        break;
    case LEVEL_EDITOR:
        m_levelEditor.proccessInput(currentGameState);
        break;

    default:
        break;
    }
}


static void centerCamera(Vector2& cameraOffset, Vector2i windowSize, Vector2i tileMapSize, int tileSize) {
    cameraOffset.x =windowSize.x/2 - (tileMapSize.x*tileSize)/2;   // coordinate x origin
    cameraOffset.y = windowSize.y/2 - (tileMapSize.y*tileSize)/2;   // coordinate y origin
}

void Game::updateGame(float deltaTime) 
{
    m_player.update(deltaTime);
    m_line.update(deltaTime);

    m_tileMap.checkAndResolveCollision(m_player);

    if (m_tileMap.checkForNextLevel(m_player.m_position)) 
        centerCamera(camera.offset, {m_windowWidth, m_windowHeight}, m_tileMap.getSize(), m_tileMap.getTileSize());
    
}

void Game::update()
{
    float deltaTime = GetFrameTime();

    switch (currentGameState)
    {
    case GAME:
        updateGame(deltaTime);
        break;
    case LEVEL_EDITOR:
        m_levelEditor.update(deltaTime);
        break;

    default:
        break;
    }
}


void Game::drawGame() {
    ClearBackground(DARKGRAY);

    Vector2 offset = {500,64*2};

    BeginMode2D (camera);  // next objects effected by Camera2D
    m_tileMap.draw();
    m_player.draw();
    EndMode2D (); 
    m_line.draw();

}

static void drawFPS() {
    std::string frameRate = std::to_string(1.0/GetFrameTime());
    DrawText(frameRate.c_str(), 0, 0, 30, BLACK);
}

void Game::draw()
{
    BeginDrawing();

        switch (currentGameState)
        {
        case GAME:
            drawGame();
            break;
        case LEVEL_EDITOR:
            m_levelEditor.draw();
            break;

        default:
            break;
        }

    
    drawFPS();

    EndDrawing();
}