#pragma once

#include <functional>
#include "app/interfaces.hpp"


class MenuScene : public IScene
{
private:
    int m_windowWidth;
    int m_windowHeight;

    bool m_showInfo = false;

    std::function<void()> m_onPlay;
    std::function<void()> m_onEditor;
    std::function<void()> m_onExit;

public:
    MenuScene(int windowWidth, int windowHeight, std::function<void()> onPlay, std::function<void()> onEditor, std::function<void()> onExit);

    void processInput() override;
    void update(float dt) override;
    void draw() override;

};

