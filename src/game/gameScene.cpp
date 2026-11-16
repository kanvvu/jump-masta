#include "gameScene.hpp"
#include  <iostream>
#include <cmath>
#include <algorithm>

GameScene::GameScene(int windowWidth, int windowHeight, Player player, LevelMap tileMap, const GameSettings& gameSettings, const PlayerSounds& playerSounds, std::function<void()> onSwitchToEditor, std::function<void()> onSwitchToMenu)
: m_windowWidth(windowWidth),
  m_windowHeight(windowHeight),
  m_player(player),
  m_tileMap(tileMap),
  m_gameSettings(gameSettings),
  m_playerSounds(playerSounds),
  m_onSwitchToEditor(onSwitchToEditor),
  m_onSwitchToMenu(onSwitchToMenu)
{
    startNewGame();
}

void GameScene::saveCurrentState() const
{
    if (m_victory) {
        std::remove("save.txt");
        return;
    }
    SaveData data;

    data.levelIndex = m_tileMap.getCurrentLevelIndex();

    data.playerPosition = m_player.m_position;
    data.playerVelocity = m_player.m_velocity;
    data.facingRight = m_player.m_facingRight;
    data.onGround = m_player.onGround;

    data.jumpCount = m_jumpCount;
    data.elapsedTime = m_elapsedTime;
    saveGame(data);
}

void GameScene::loadFromSave(const SaveData &data)
{
    m_suppressLandSound = false;

    m_tileMap.changeLevel(data.levelIndex);

    m_player.m_position = data.playerPosition;
    m_player.m_previousPosition = data.playerPosition;
    m_player.m_velocity =  data.playerVelocity;
    m_player.m_facingRight = data.facingRight;
    m_player.onGround = data.onGround;

    m_jumpCount = data.jumpCount;
    m_elapsedTime = data.elapsedTime;
    m_victory = false;

    m_line.reset();

    camera.offset = calculateCameraCenterOffset({m_windowWidth, m_windowHeight}, m_tileMap.getSize(), m_tileMap.getTileSize());
}

void GameScene::startNewGame()
{
    m_suppressLandSound = true;

    m_tileMap.changeLevel(m_tileMap.getStartLevelIndex());

    camera.offset = calculateCameraCenterOffset({m_windowWidth, m_windowHeight}, m_tileMap.getSize(), m_tileMap.getTileSize());
    camera.target.x = 0;         // rotation and zoom x origin (from offset above)
    camera.target.y = 0;         // rotation and zoom y origin (from offset above)
    camera.rotation = 0;        // rotation in deg
    camera.zoom     = 1.0f;      // magnification, i.e fov or zoom


    m_player.reset(m_tileMap.getSpawnPosition());

    m_jumpCount = 0;
    m_elapsedTime = 0;
    m_victory = false;

    m_line.reset();
}

void GameScene::processInput()
{
    
    if (IsKeyPressed(KEY_ESCAPE)) {
        m_line.reset();
        saveCurrentState();
        m_onSwitchToMenu();
        return;
    }
    
    if (m_victory) return;

    if (IsKeyPressed(KEY_R)) {
        startNewGame();
    }

    if (IsMouseButtonDown(0)) {
        m_line.mouseButtonDown();
        m_player.m_isAiming = true;
    }

    // std::cout<<m_player.onGround<<"\n";
    if (!IsMouseButtonDown(0)) {
        m_player.m_isAiming = false;
        if (auto impulse = m_line.mouseButtonUp()) {
            if (m_player.onGround) {
                m_player.m_velocity = *impulse; 
                m_player.onGround = false;

                m_jumpCount++;
                PlaySound(m_playerSounds.jump);
            }
        }
    }
}

void GameScene::update(float deltaTime)
{
    if (m_victory) return;

    if (deltaTime > 0.1f) deltaTime = 0.1f;

    m_elapsedTime += deltaTime;

    float maxX = std::abs(m_player.m_velocity.x) * deltaTime;
    float maxY = (std::abs(m_player.m_velocity.y) + PlayerConfig::GRAVITY * deltaTime)*deltaTime;
    float maxDistance = std::max(maxX, maxY);
    const float MAX_STEP_TILES = 0.4f;
    int steps = std::max(1, (int) std::ceil(maxDistance/MAX_STEP_TILES));
    float stepDt = deltaTime / steps;

    bool wasOnGround = m_player.onGround;
    bool playerHitWall = false;
    for(int i=0; i<steps; i++) {
        m_player.update(stepDt);
        m_tileMap.checkAndResolveCollision(m_player);
        if (m_player.m_hitWall) playerHitWall = true;
    }

    m_line.update(deltaTime);

    if (!wasOnGround && m_player.onGround) {
        if (m_suppressLandSound)
            m_suppressLandSound = false;
        else 
            PlaySound(m_playerSounds.land);
    }

    if (playerHitWall) {
        PlaySound(m_playerSounds.bump);
    }

    if (m_tileMap.isGoalReached(m_player.m_position)) {
        m_player.m_isAiming = false;
        m_victory = true;
        std::remove("save.txt");
        m_line.reset();
        return;
    }

    if (m_tileMap.checkForNextLevel(m_player.m_position)) { 
        std::cout<<"NEXT LEVEL\n";
        camera.offset = calculateCameraCenterOffset({m_windowWidth, m_windowHeight}, m_tileMap.getSize(), m_tileMap.getTileSize());
        // centerCamera(camera.offset, {m_windowWidth, m_windowHeight}, m_tileMap.getSize(), m_tileMap.getTileSize());
        saveCurrentState();
    }
}

void GameScene::draw()
{
    ClearBackground(DARKGRAY);

    Vector2 offset = {500,64*2};


    BeginMode2D (camera);  // next objects effected by Camera2D

        m_tileMap.draw();
        m_player.draw(m_tileMap.getTileSize());

    EndMode2D (); 

    m_line.draw();

    if (m_gameSettings.showTimer) {
        char buffer[64];
        int ret = snprintf(buffer, sizeof buffer, "Time: %.2f s", m_elapsedTime);
        if (ret >= 0) 
            DrawText(buffer, 0, 30, 30, RAYWHITE);
    }

    if (m_gameSettings.showJumpCount) {
        std::string jumpCount = "Jumps: " + std::to_string(m_jumpCount);
        DrawText(jumpCount.c_str(), 0, 60, 30, RAYWHITE);
    }

    if (m_victory) {

        Vector2 backgroundSize{800, 310};
        Vector2 backgroundPosition{m_windowWidth / 2.0f - backgroundSize.x/ 2.0f, 300};
        DrawRectangle((int) backgroundPosition.x,(int) backgroundPosition.y,(int) backgroundSize.x,(int) backgroundSize.y, {0, 0, 0, 162});

        std::string text = "YOU WON!!!!";
        int fontSize = 100;
        int textWidth = MeasureText(text.c_str(), fontSize);
        Vector2i textPosition { m_windowWidth / 2 - textWidth / 2,(int) backgroundPosition.y + 10};
        DrawText(text.c_str(), textPosition.x,textPosition.y, fontSize, RAYWHITE);

        int textOffset = 50;

        char buffer[64];
        int ret = snprintf(buffer, sizeof buffer, "Total Time: %.2f s", m_elapsedTime);
        if (ret >= 0) { 
            fontSize = 50;
            textWidth = MeasureText(buffer, fontSize);
            textPosition  = { m_windowWidth / 2 - textWidth / 2, textPosition.y + textOffset + 80};

            DrawText(buffer, textPosition.x, textPosition.y, fontSize, RAYWHITE);
        }

        text = "Total Jumps: " + std::to_string(m_jumpCount);
        fontSize = 50;
        textWidth = MeasureText(text.c_str(), fontSize);
        textPosition  = { m_windowWidth / 2 - textWidth / 2, textPosition.y + textOffset};

        DrawText(text.c_str(), textPosition.x,textPosition.y, fontSize, RAYWHITE);

        text = "(Press ESC to return to Main Menu)";
        fontSize = 30;
        textWidth = MeasureText(text.c_str(), fontSize);
        textPosition  = { m_windowWidth / 2 - textWidth / 2, textPosition.y + textOffset + 30};

        DrawText(text.c_str(), textPosition.x,textPosition.y, fontSize, RAYWHITE);
    }

}
