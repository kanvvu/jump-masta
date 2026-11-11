#pragma once

#include "app/interfaces.hpp"
#include "level/levelMap.hpp"
#include "line.hpp"
#include <functional>

class GameScene : public IScene {
private:
    int m_windowWidth;
    int m_windowHeight;


    LevelMap m_tileMap;
    Player m_player;
    Line m_line;

    Camera2D camera;

    std::function<void()> m_onSwitchToEditor;
    std::function<void()> m_onSwitchToMenu;

public:
    GameScene(int windowWidth, int windowHeight, Player player, LevelMap tileMap, std::function<void()> onSwitchToEditor, std::function<void()> onSwitchToMenu);
    void processInput() override;
    void update(float deltaTime) override;
    void draw() override;

};