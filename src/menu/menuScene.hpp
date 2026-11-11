#pragma once

#include <functional>
#include "app/interfaces.hpp"
#include "app/common.hpp"


class MenuScene : public IScene
{
private:
    int m_windowWidth;
    int m_windowHeight;

    bool m_showInfo = false;
    bool m_showSettings = false;

    std::function<void()> m_onPlay;
    std::function<void()> m_onEditor;
    std::function<void()> m_onExit;

    GameSettings& m_gameSettings;

public:
    MenuScene(int windowWidth, int windowHeight, GameSettings& gameSettings, std::function<void()> onPlay, std::function<void()> onEditor, std::function<void()> onExit);

    void processInput() override;
    void update(float dt) override;
    void draw() override;

};

