#pragma once
#include "raylib.h"
#include "levelMap.hpp"
#include "player.hpp"
#include "line.hpp"
#include "levelEditor.hpp"
#include "common.hpp"
#include "levelFileLoader.hpp"


class Game
{
private:
    int m_windowWidth;
    int m_windowHeight;
    const char *m_windowTitle;
    Camera2D camera = {0};
    GameState currentGameState = LEVEL_EDITOR;

    LevelFileLoader m_levelFileLoader;

    LevelRepository m_levelRepository;

    LevelEditor m_levelEditor;

    LevelMap m_tileMap;

    Player m_player;

    Line m_line;


public:
    Game(int windowWidth, int windowHeight, const char *windowTitle);
    ~Game();

    void start();
    void proccessInput();
    void update();
    void draw();

    void proccessInputGame();
    void updateGame(float deltaTime);
    void drawGame();
};

