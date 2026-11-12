#pragma once

#include "app/interfaces.hpp"
#include "level/levelMap.hpp"
#include "line.hpp"
#include <functional>
#include "app/common.hpp"
#include "game/saveSystem.hpp"

class GameScene : public IScene {
private:
    int m_windowWidth;
    int m_windowHeight;

    bool m_victory = false;


    LevelMap m_tileMap;
    Player m_player;
    Line m_line;

    Camera2D camera;

    std::function<void()> m_onSwitchToEditor;
    std::function<void()> m_onSwitchToMenu;

    int m_jumpCount = 0;
    float m_elapsedTime = 0.0f;

    const GameSettings& m_gameSettings;

public:
    GameScene(int windowWidth, int windowHeight, Player player, LevelMap tileMap, const GameSettings& gameSettings, std::function<void()> onSwitchToEditor, std::function<void()> onSwitchToMenu);

    void saveCurrentState() const;

    void loadFromSave(const SaveData& data);
    void startNewGame();

    void processInput() override;
    void update(float deltaTime) override;
    void draw() override;

};