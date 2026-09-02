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

    camera.offset.x = windowWidth/2 - (10*64)/2;   // coordinate x origin
    camera.offset.y = windowHeight/2 - (15*64)/2;   // coordinate y origin
    camera.target.x = 0;         // rotation and zoom x origin (from offset above)
    camera.target.y = 0;         // rotation and zoom y origin (from offset above)
    camera.rotation = 0;        // rotation in deg
    camera.zoom     = 1.0f;      // magnification, i.e fov or zoom

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

void Game::proccessInputGame() {
    if (IsKeyPressed(KEY_E)) {
        currentGameState = LEVEL_EDITOR;
    }   

    Vector2 dir = {0.0, 0.0};
    if (IsKeyDown(KEY_W)) { dir.y = -1;}
    if (IsKeyDown(KEY_A)) { dir.x = -1;}
    if (IsKeyDown(KEY_S)) { dir.y = 1;}
    if (IsKeyDown(KEY_D)) { dir.x = 1;}

    if (IsKeyPressed(KEY_R)) {
        m_player.reset();
        m_player.m_position = m_tileMap.getSpawnPosition();
    }

    // m_player.m_direction = Vector2Normalize(dir);

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

static void checkAndResolveCollision(Player& player, const TileMap& tileMap) {
    Vector2 bP = player.m_previousPosition; 
    Vector2 nBP = player.m_position;
    
    // velo = Vector2Subtract(nBP, bP);
    // std::string msg = "";
    Vector2 movementDirection = Vector2Subtract(nBP, bP);
    if (movementDirection.x <= 0) { //left
        if (tileMap.isCollision({nBP.x, bP.y}, 1) ||
            tileMap.isCollision({nBP.x, bP.y + 0.99f}, 1)) {
            nBP.x = (int) nBP.x + 1;
            player.m_velocity.x = -0.5*player.m_velocity.x;
            // std::cout<<"left\n";
            // msg += "| left (" + std::to_string(nBP.x) + ", " + std::to_string(nBP.y) + "), " + "(" + std::to_string(m_player.m_velocity.x) + ", " + std::to_string(m_player.m_velocity.y) + ") ";
            }
    }
    else {  //right
        if (tileMap.isCollision({nBP.x + 1.0f, bP.y}, 1) ||
            tileMap.isCollision({nBP.x + 1.0f, bP.y + 0.99f}, 1)) {
            nBP.x = (int) nBP.x;
            player.m_velocity.x = -0.5*player.m_velocity.x;
            // std::cout<<"right\n";
            // msg += "| right ";
        }
    }

    player.onGround = false;
    if (movementDirection.y <= 0) { //up
        if (tileMap.isCollision({nBP.x, nBP.y}, 1) ||
            tileMap.isCollision({nBP.x + 0.90f, nBP.y}, 1)) {
            nBP.y = (int) nBP.y + 1;
            player.m_velocity.y = -0.5*player.m_velocity.y;
            // std::cout<<"up " <<"("<<nBP.x << ", " << nBP.y <<"), "<< "("<<m_player.m_velocity.x << ", " << m_player.m_velocity.y <<"), " << "\n";
            // msg += "| up (" + std::to_string(nBP.x) + ", " + std::to_string(nBP.y) + "), " + "(" + std::to_string(m_player.m_velocity.x) + ", " + std::to_string(m_player.m_velocity.y) + ") ";
        }
    }
    else { //down
        if (tileMap.isCollision({nBP.x, nBP.y + 1.0f}, 1) ||
            tileMap.isCollision({nBP.x + 0.90f, nBP.y + 1.0f}, 1)) {
            nBP.y = (int) nBP.y;
            player.m_velocity.y = 0;
            // std::cout<<"down\n";
            // msg += "| down ";
            player.onGround = true;
        }
    }
    // if (msg != "") std::cout<<msg<<"\n";

    player.m_position = nBP;
}

void Game::updateGame(float deltaTime) 
{
    m_player.update(deltaTime);
    m_line.update(deltaTime);

    checkAndResolveCollision(m_player, m_tileMap);
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
        m_levelEditor.update();
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

    std::string frameRate = std::to_string(1.0/GetFrameTime());
    DrawText(frameRate.c_str(), 0, 0, 30, BLACK);

    EndDrawing();
}