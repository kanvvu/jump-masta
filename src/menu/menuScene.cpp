#include "menuScene.hpp"

#include "raylib.h"
#include <string>
#include <third_party/raygui.h>
#include "app/common.hpp"

MenuScene::MenuScene(int windowWidth, int windowHeight, std::function<void()> onPlay, std::function<void()> onEditor, std::function<void()> onExit)
: m_windowWidth(windowWidth),
  m_windowHeight(windowHeight),
  m_onPlay(onPlay),
  m_onEditor(onEditor),
  m_onExit(onExit)
{
}

void MenuScene::processInput() {
    if (m_showInfo && IsKeyPressed(KEY_ESCAPE)) {
        m_showInfo = false;
    }
}

void MenuScene::update(float dt) {
}

void MenuScene::draw() {
    ClearBackground(DARKGRAY);
    GuiSetStyle(DEFAULT, TEXT_SIZE, GuiGetFont().baseSize*3);


    std::string title = "JUMP MASTA!";
    int titleFontSize = 60;
    int titleWidth = MeasureText(title.c_str(), titleFontSize);
    Vector2i titlePosition {m_windowWidth / 2 - titleWidth/ 2, 200};
    DrawText(title.c_str(), titlePosition.x, titlePosition.y, titleFontSize, WHITE);

    Vector2 buttonSize {200.0f, 50.0f};
    float centerX = m_windowWidth / 2.0f - buttonSize.x / 2.0f;

    if (!m_showInfo) {
        float offsetFromTitle = 200.0f;

        //how do i set a font size for GuiButton?
        Rectangle buttonBounds {centerX, titlePosition.y + offsetFromTitle, buttonSize.x, buttonSize.y};
        if (GuiButton(buttonBounds,"PLAY"))
            m_onPlay();

        int offsetFromButton = 80;
        buttonBounds = {centerX, buttonBounds.y + offsetFromButton, buttonSize.x, buttonSize.y};
        if (GuiButton(buttonBounds,"EDITOR"))
            m_onEditor();

        buttonBounds = {centerX, buttonBounds.y + offsetFromButton, buttonSize.x, buttonSize.y};
        if (GuiButton(buttonBounds,"INFO"))
            m_showInfo = true;

        buttonBounds = {centerX, buttonBounds.y + offsetFromButton, buttonSize.x, buttonSize.y};
        if (GuiButton(buttonBounds,"EXIT"))
            m_onExit();

    } else {

        Vector2 backgroundSize{400, 400};
        Vector2 backgroundPosition{m_windowWidth / 2.0f - backgroundSize.x/ 2.0f, m_windowHeight / 2.0f - backgroundSize.y/ 2.0f};
        DrawRectangle((int) backgroundPosition.x,(int) backgroundPosition.y,(int) backgroundSize.x,(int) backgroundSize.y, WHITE);
        
        std::string infoTitle = "Controls:";
        int infoTitleFontSize = 30;
        Vector2i textPosition {(int) backgroundPosition.x + 10,(int) backgroundPosition.y + 10};
        DrawText(infoTitle.c_str(), textPosition.x, textPosition.y, infoTitleFontSize, BLACK);

        int textFontSize = 20;
        int textOffset = 40;
        int titleOffset = 60;
        int padding = 10;
        textPosition = {(int) backgroundPosition.x + padding, textPosition.y + titleOffset};
        DrawText("Hold & Drag Left Mouse: Jump", textPosition.x, textPosition.y, textFontSize, BLACK);

        textPosition = {(int) backgroundPosition.x + padding, textPosition.y + textOffset};
        DrawText("R: Reset Level", textPosition.x, textPosition.y, textFontSize, BLACK);

        textPosition = {(int) backgroundPosition.x + padding, textPosition.y + textOffset};
        DrawText("ESC: Return to Menu", textPosition.x, textPosition.y, textFontSize, BLACK);

        Rectangle buttonBounds {backgroundPosition.x + padding, backgroundPosition.y + backgroundSize.y - buttonSize.y - 10,  buttonSize.x, buttonSize.y};
        if (GuiButton(buttonBounds,"BACK"))
            m_showInfo = false;


    }
};