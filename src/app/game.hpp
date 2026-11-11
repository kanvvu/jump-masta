#pragma once
#include "raylib.h"
#include "level/levelMap.hpp"
#include "game/gameScene.hpp"
#include "editor/levelEditor.hpp"
#include "level/levelFileLoader.hpp"
#include "assets/assets.hpp"
#include <memory>
#include "menu/menuScene.hpp"


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
    const char *m_windowTitle;

    LevelFileLoader m_levelFileLoader;
    LevelRepository m_levelRepository;
    Assets m_assets;

    std::unique_ptr<LevelEditor> m_levelEditor;
    std::unique_ptr<GameScene> m_gameScene;
    std::unique_ptr<MenuScene> m_menuScene;

    IScene* m_currentScene = nullptr;

    bool m_running = true;

    GameSettings m_gameSettings;

public:
    Game(int windowWidth, int windowHeight, const char *windowTitle);
    ~Game() = default;


    void start();

    void proccessInput();
    void update();
    void draw();

    void onSwitchToGame();
    void onSwitchToEditor();
    void onSwitchToExit();
    void onSwitchToMenu();
};

