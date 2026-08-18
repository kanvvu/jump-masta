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

    // m_player.m_direction = Vector2Normalize(dir);

    if (IsMouseButtonDown(0) && !isLeftMouseButtonPressed)
    {
        isLeftMouseButtonPressed = true;
        startLinePos = GetMousePosition();
    }

    if (!IsMouseButtonDown(0)) {
        if (isLeftMouseButtonPressed) {
            m_player.m_velocity = Vector2Subtract(GetMousePosition(), startLinePos);
            m_player.m_velocity = Vector2Scale(m_player.m_velocity, 0.07);
            // printf("realsed! (%f, %f)\n", velo.x, velo.y);
        }
        isLeftMouseButtonPressed = false;
    }

    if (isLeftMouseButtonPressed){
        float lineLength = Vector2Length(Vector2Subtract(GetMousePosition(), startLinePos)); 
        //printf("%f\n", lineLength);

        if (lineLength < 300.0) lineColor = RED;
        if (lineLength < 200.0) lineColor = ORANGE;
        if (lineLength < 100.0) lineColor = BLACK;
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

void Game::update()
{
    m_player.update(GetFrameTime());

    checkAndResolveCollision(m_player, m_tileMap);

}


void Game::draw()
{
    BeginDrawing();
    ClearBackground(DARKGRAY);

    m_tileMap.draw();
    m_player.draw();

    std::string frameRate = std::to_string(1.0/GetFrameTime());
    DrawText(frameRate.c_str(), 0, 0, 30, BLACK);
    if (isLeftMouseButtonPressed) {
        // Vector2 newStartLinePos = Vector2Subtract(Vector2Scale(m_player.m_position, 64), startLinePos);
        // DrawLineEx(Vector2Scale(m_player.m_position, 64), Vector2Add(GetMousePosition(), newStartLinePos), 3, lineColor);
        DrawLineEx(startLinePos, GetMousePosition(), 3, lineColor);
    }

    EndDrawing();
}