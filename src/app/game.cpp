#include "game.hpp"
#include "raymath.h"
#include <iostream>
#include <string>

Game::Game(int windowWidth, int windowHeight, const char *windowTitle)
  : m_raylibContext(windowWidth, windowHeight, windowTitle),
    // m_windowWidth(windowWidth),
    // m_windowHeight(windowHeight),
    m_windowTitle(windowTitle),
    m_levelRepository(m_levelFileLoader),
    m_assets(),
    m_levelEditor(std::make_unique<LevelEditor>(m_levelRepository, m_assets,[this]() {onSwitchToGame();})),
    m_gameScene(std::make_unique<GameScene>(windowWidth, windowHeight, Player(m_assets.getPlayer()), LevelMap(m_levelRepository, m_assets), [this] () {onSwitchToEditor();}))
{
    SetTargetFPS(60);

    m_currentScene = m_levelEditor.get();

    for(int i=0; i<m_levelRepository.getLevelCount(); i++) m_levelRepository.getLevel(i).rebuildCollision(m_assets);
}

void Game::onSwitchToGame()
{
    m_currentScene = m_gameScene.get();
}

void Game::onSwitchToEditor()
{
    m_currentScene = m_levelEditor.get();
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
    if (m_currentScene) m_currentScene->processInput();
}

void Game::update()
{
    float deltaTime = GetFrameTime();
    if (m_currentScene) m_currentScene->update(deltaTime);
}

static void drawFPS() {
    std::string frameRate = std::to_string(1.0/GetFrameTime());
    DrawText(frameRate.c_str(), 0, 0, 30, BLACK);
}

void Game::draw()
{
    BeginDrawing();

    m_currentScene->draw();
    drawFPS();

    EndDrawing();
}