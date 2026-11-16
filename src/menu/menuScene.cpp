#include "menuScene.hpp"

#include "raylib.h"
#include <string>
#include <third_party/raygui.h>
#include "app/common.hpp"
#include "game/saveSystem.hpp"

MenuScene::MenuScene(int windowWidth, int windowHeight, GameSettings& gameSettings, std::function<void()> onPlay, std::function<void()> onEditor, std::function<void()> onExit, std::function<void()> onContinue)
: m_windowWidth(windowWidth),
  m_windowHeight(windowHeight),
  m_gameSettings(gameSettings),
  m_onPlay(onPlay),
  m_onEditor(onEditor),
  m_onExit(onExit),
  m_onContinue(onContinue)
{
    m_hasSave = hasSaveGame();
}

void MenuScene::processInput() {
    if ((m_showInfo || m_showSettings) && IsKeyPressed(KEY_ESCAPE)) {
        m_showInfo = false;
        m_showSettings = false;
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

    if (!m_showInfo && !m_showSettings) {
        float offsetFromTitle = 200.0f;

        //how do i set a font size for GuiButton?
        Rectangle buttonBounds {centerX, titlePosition.y + offsetFromTitle, buttonSize.x, buttonSize.y};
        if (GuiButton(buttonBounds,"NEW GAME"))
            m_onPlay();

        int offsetFromButton = 80;
        if(m_hasSave) {
            buttonBounds = {centerX, buttonBounds.y + offsetFromButton, buttonSize.x, buttonSize.y};
            if (GuiButton(buttonBounds,"CONTINUE"))
                m_onContinue();
        }

        buttonBounds = {centerX, buttonBounds.y + offsetFromButton, buttonSize.x, buttonSize.y};
        if (GuiButton(buttonBounds,"EDITOR"))
            m_onEditor();

        buttonBounds = {centerX, buttonBounds.y + offsetFromButton, buttonSize.x, buttonSize.y};
        if (GuiButton(buttonBounds,"SETTINGS"))
            m_showSettings = true;

        buttonBounds = {centerX, buttonBounds.y + offsetFromButton, buttonSize.x, buttonSize.y};
        if (GuiButton(buttonBounds,"INFO"))
            m_showInfo = true;

        #ifndef __EMSCRIPTEN__
            buttonBounds = {centerX, buttonBounds.y + offsetFromButton, buttonSize.x, buttonSize.y};
            if (GuiButton(buttonBounds,"EXIT"))
                m_onExit();
        #endif

    }
    else if (m_showInfo) {

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

    } else {
        GuiSetStyle(DEFAULT, TEXT_SIZE, GuiGetFont().baseSize*2);

        Vector2 backgroundSize{400, 400};
        Vector2 backgroundPosition{m_windowWidth / 2.0f - backgroundSize.x/ 2.0f, m_windowHeight / 2.0f - backgroundSize.y/ 2.0f};
        DrawRectangle((int) backgroundPosition.x,(int) backgroundPosition.y,(int) backgroundSize.x,(int) backgroundSize.y, WHITE);
        
        std::string settingsTitle = "Settings:";
        int infoTitleFontSize = 30;
        Vector2i textPosition {(int) backgroundPosition.x + 10,(int) backgroundPosition.y + 10};
        DrawText(settingsTitle.c_str(), textPosition.x, textPosition.y, infoTitleFontSize, BLACK);

        int textFontSize = 20;
        int textOffset = 40;
        int titleOffset = 60;
        int padding = 10;

        float toggleX = backgroundPosition.x + 300.0f;

        textPosition = {(int) backgroundPosition.x + padding, textPosition.y + titleOffset};
        std::string text = "Show FPS: ";
        DrawText(text.c_str(), textPosition.x, textPosition.y, textFontSize, BLACK);
        if (GuiToggleSlider({toggleX,(float) textPosition.y - textFontSize/4.0f, 90.0f, 30.0f }, "OFF;ON", &m_gameSettings.showFps)) { }

        textPosition = {(int) backgroundPosition.x + padding, textPosition.y + textOffset};
        text = "Show time: ";
        DrawText(text.c_str(), textPosition.x, textPosition.y, textFontSize, BLACK);
        if (GuiToggleSlider({toggleX,(float) textPosition.y - textFontSize/4.0f, 90.0f, 30.0f }, "OFF;ON", &m_gameSettings.showTimer)) { }

        textPosition = {(int) backgroundPosition.x + padding, textPosition.y + textOffset};
        text = "Show jump counter: ";
        DrawText(text.c_str(), textPosition.x, textPosition.y, textFontSize, BLACK);
        if (GuiToggleSlider({toggleX,(float) textPosition.y - textFontSize/4.0f, 90.0f, 30.0f }, "OFF;ON", &m_gameSettings.showJumpCount)) { }

        Rectangle buttonBounds {backgroundPosition.x + padding, backgroundPosition.y + backgroundSize.y - buttonSize.y - 10,  buttonSize.x, buttonSize.y};
        if (GuiButton(buttonBounds,"BACK"))
            m_showSettings = false;

    }

};