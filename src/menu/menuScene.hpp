#pragma once

#include <functional>
#include "app/interfaces.hpp"
#include "app/common.hpp"
#include "game/saveSystem.hpp"


class MenuScene : public IScene
{
private:
    int m_windowWidth;
    int m_windowHeight;

    bool m_showInfo = false;
    bool m_showSettings = false;
    bool m_hasSave = false;

    std::function<void()> m_onPlay;
    std::function<void()> m_onEditor;
    std::function<void()> m_onExit;
    std::function<void()> m_onContinue;

    GameSettings& m_gameSettings;

public:
    MenuScene(int windowWidth, int windowHeight, GameSettings& gameSettings, std::function<void()> onPlay, std::function<void()> onEditor, std::function<void()> onExit, std::function<void()> onContinue);

    void refreshSaveState() { m_hasSave = hasSaveGame(); }

    void processInput() override;
    void update(float dt) override;
    void draw() override;

};

