#include "game.hpp"
#include "raymath.h"
#include <iostream>
#include <string>
#include "game/saveSystem.hpp"

Game::Game(int windowWidth, int windowHeight, const char *windowTitle)
  : m_raylibContext(windowWidth, windowHeight, windowTitle),
    // m_windowWidth(windowWidth),
    // m_windowHeight(windowHeight),
    m_windowTitle(windowTitle),
    m_levelRepository(m_levelFileLoader),
    m_assets(),
    m_levelEditor(std::make_unique<LevelEditor>(m_levelRepository, m_assets,[this]() {onSwitchToNewGame();}, [this]() {onSwitchToMenu();})),
    m_gameScene(std::make_unique<GameScene>(windowWidth, windowHeight, Player(m_assets.getPlayer()), LevelMap(m_levelRepository, m_assets), m_gameSettings, [this] () {onSwitchToEditor();}, [this] () {onSwitchToMenu();})),
    m_menuScene(std::make_unique<MenuScene>(windowWidth, windowHeight, m_gameSettings, [this]() {onSwitchToNewGame();}, [this]() {onSwitchToEditor();}, [this]() {onSwitchToExit();}, [this]() {onSwitchToLoadGame();}))
{
    SetTargetFPS(60);
    SetExitKey(KEY_NULL);

    m_currentScene = m_menuScene.get();

    for(int i=0; i<m_levelRepository.getLevelCount(); i++) m_levelRepository.getLevel(i).rebuildCollision(m_assets);
}

void Game::onSwitchToNewGame()
{
    m_currentScene = m_gameScene.get();
    m_gameScene->startNewGame();
}

void Game::onSwitchToLoadGame()
{
    m_currentScene = m_gameScene.get();

    SaveData data;
    if (loadGame(data))
        m_gameScene->loadFromSave(data);
    else
        m_gameScene->startNewGame();

}

void Game::onSwitchToEditor()
{
    m_currentScene = m_levelEditor.get();
}

void Game::onSwitchToExit()
{
    m_running = false;
}

void Game::onSwitchToMenu()
{
    m_currentScene = m_menuScene.get();
    m_menuScene->refreshSaveState();
}

void Game::start()
{
    while (!WindowShouldClose() && m_running)
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
    std::string frameRate = std::to_string(GetFPS());
    DrawText(frameRate.c_str(), 0, 0, 30, RAYWHITE);
}

void Game::draw()
{
    BeginDrawing();

    m_currentScene->draw();
    if (m_gameSettings.showFps)
        drawFPS();

    EndDrawing();
}