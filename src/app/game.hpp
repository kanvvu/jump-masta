#pragma once
#include "raylib.h"
#include "level/levelMap.hpp"
#include "game/player.hpp"
#include "game/line.hpp"
#include "editor/levelEditor.hpp"
#include "common.hpp"
#include "level/levelFileLoader.hpp"
#include "assets/assets.hpp"

struct RaylibContext {
    RaylibContext(int w, int h, const char* title) {
        InitWindow(w, h, title);
    }
    ~RaylibContext() {
        CloseWindow();
    }
};

class Game
{
private:
    RaylibContext m_raylibContext;
    int m_windowWidth;
    int m_windowHeight;
    const char *m_windowTitle;
    Camera2D camera = {0};
    GameState currentGameState = GameState::LEVEL_EDITOR;

    LevelFileLoader m_levelFileLoader;

    LevelRepository m_levelRepository;

    Assets m_assets;

    LevelEditor m_levelEditor;

    LevelMap m_tileMap;

    Player m_player;

    Line m_line;


public:
    Game(int windowWidth, int windowHeight, const char *windowTitle);
    ~Game() = default;

    void start();
    void proccessInput();
    void update();
    void draw();

    void proccessInputGame();
    void updateGame(float deltaTime);
    void drawGame();
};

