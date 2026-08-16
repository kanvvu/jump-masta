#pragma once
#include "raylib.h"
#include "tileMap.hpp"
#include "player.hpp"

class Game
{
private:
    int m_windowWidth;
    int m_windowHeight;
    const char *m_windowTitle;

    TileMap m_tileMap;

    Player m_player;


public:
    Game(int windowWidth, int windowHeight, const char *windowTitle);
    ~Game();

    void start();
    void proccessInput();
    void update();
    void draw();
};

