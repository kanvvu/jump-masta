#include "gameScene.hpp"
#include  <iostream>

GameScene::GameScene(int windowWidth, int windowHeight, Player player, LevelMap tileMap, const GameSettings& gameSettings, std::function<void()> onSwitchToEditor, std::function<void()> onSwitchToMenu)
: m_windowWidth(windowWidth),
  m_windowHeight(windowHeight),
  m_player(player),
  m_tileMap(tileMap),
  m_gameSettings(gameSettings),
  m_onSwitchToEditor(onSwitchToEditor),
  m_onSwitchToMenu(onSwitchToMenu)
{
    camera.offset = calculateCameraCenterOffset({windowWidth, windowHeight}, m_tileMap.getSize(), m_tileMap.getTileSize());
    camera.target.x = 0;         // rotation and zoom x origin (from offset above)
    camera.target.y = 0;         // rotation and zoom y origin (from offset above)
    camera.rotation = 0;        // rotation in deg
    camera.zoom     = 1.0f;      // magnification, i.e fov or zoom

    m_player.m_position = m_tileMap.getSpawnPosition();
    m_player.m_previousPosition = m_player.m_position;
}

void GameScene::saveCurrentState() const
{
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
    m_tileMap.changeLevel(data.levelIndex);

    m_player.m_position = data.playerPosition;
    m_player.m_previousPosition = data.playerPosition;
    m_player.m_velocity =  data.playerVelocity;
    m_player.m_facingRight = data.facingRight;
    m_player.onGround = data.onGround;

    m_jumpCount = data.jumpCount;
    m_elapsedTime = data.elapsedTime;

    camera.offset = calculateCameraCenterOffset({m_windowWidth, m_windowHeight}, m_tileMap.getSize(), m_tileMap.getTileSize());
}

void GameScene::startNewGame()
{
    camera.offset = calculateCameraCenterOffset({m_windowWidth, m_windowHeight}, m_tileMap.getSize(), m_tileMap.getTileSize());
    camera.target.x = 0;         // rotation and zoom x origin (from offset above)
    camera.target.y = 0;         // rotation and zoom y origin (from offset above)
    camera.rotation = 0;        // rotation in deg
    camera.zoom     = 1.0f;      // magnification, i.e fov or zoom

    m_player.m_position = m_tileMap.getSpawnPosition();
    m_player.m_previousPosition = m_player.m_position;

    m_jumpCount = 0;
    m_elapsedTime = 0;
    m_player.reset();
}

void GameScene::processInput()
{
    if (IsKeyPressed(KEY_E)) {
        m_onSwitchToEditor();
    }   

    if (IsKeyPressed(KEY_ESCAPE)) {
        saveCurrentState();
        m_onSwitchToMenu();
    }

    if (IsKeyPressed(KEY_R)) {
        m_player.reset();
        m_tileMap.resetLevel();
        m_player.m_position = m_tileMap.getSpawnPosition();
        m_jumpCount = 0;
        m_elapsedTime = 0.0f;
    }

    if (IsMouseButtonDown(0)) {
        m_line.mouseButtonDown();
    }

    // std::cout<<m_player.onGround<<"\n";
    if (!IsMouseButtonDown(0)) {
        if (auto impulse = m_line.mouseButtonUp()) {
            if (m_player.onGround) {
                m_player.m_velocity = *impulse; 
                m_player.onGround = false;

                m_jumpCount++;
            }
        }
    }
}

void GameScene::update(float deltaTime)
{
    m_elapsedTime += deltaTime;

    m_player.update(deltaTime);
    m_line.update(deltaTime);

    m_tileMap.checkAndResolveCollision(m_player);

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

}
