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

        BeginDrawing();
        ClearBackground(DARKGRAY);
        DrawText("Loading...", w/2 - MeasureText("Loading...", 60)/2, h/2 - 60/2, 60, WHITE);
        EndDrawing();

        InitAudioDevice();
    }
    ~RaylibContext() {
        CloseAudioDevice();
        CloseWindow();
    }
    
    RaylibContext(const RaylibContext&) = delete;
    RaylibContext& operator=(const RaylibContext&) = delete;
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

    void onSwitchToNewGame();
    void onSwitchToLoadGame();
    void onSwitchToEditor();
    void onSwitchToExit();
    void onSwitchToMenu();
};

