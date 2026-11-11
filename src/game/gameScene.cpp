#include "gameScene.hpp"
#include  <iostream>

GameScene::GameScene(int windowWidth, int windowHeight, Player player, LevelMap tileMap, std::function<void()> onSwitchToEditor, std::function<void()> onSwitchToMenu)
: m_windowWidth(windowWidth),
  m_windowHeight(windowHeight),
  m_player(player),
  m_tileMap(tileMap),
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

void GameScene::processInput()
{
    if (IsKeyPressed(KEY_E)) {
        m_onSwitchToEditor();
    }   

    if (IsKeyPressed(KEY_ESCAPE)) {
        m_onSwitchToMenu();
    }

    if (IsKeyPressed(KEY_R)) {
        m_player.reset();
        m_tileMap.resetLevel();
        m_player.m_position = m_tileMap.getSpawnPosition();
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
            }
        }
    }
}

void GameScene::update(float deltaTime)
{
    m_player.update(deltaTime);
    m_line.update(deltaTime);

    m_tileMap.checkAndResolveCollision(m_player);

    if (m_tileMap.checkForNextLevel(m_player.m_position)) { 
        std::cout<<"NEXT LEVEL\n";
        camera.offset = calculateCameraCenterOffset({m_windowWidth, m_windowHeight}, m_tileMap.getSize(), m_tileMap.getTileSize());
        // centerCamera(camera.offset, {m_windowWidth, m_windowHeight}, m_tileMap.getSize(), m_tileMap.getTileSize());
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
}
