

#include <cstdio>
#include <cassert>
#include "game/saveSystem.hpp"

void testSaveSystem() {
    SaveData saved;
    saved.elapsedTime = 31.2f;
    saved.facingRight = false;
    saved.onGround = true;
    saved.jumpCount = 17;
    saved.levelIndex = 5;
    saved.playerPosition = {12.5f, 8.0f};
    
    assert(saveGame(saved, "test_save.txt"));
    assert(hasSaveGame("test_save.txt"));


    SaveData load;
    assert(loadGame(load, "test_save.txt"));
    assert(load.elapsedTime == saved.elapsedTime);
    assert(load.facingRight == saved.facingRight);
    assert(load.onGround == saved.onGround);
    assert(load.jumpCount == saved.jumpCount);
    assert(load.levelIndex == saved.levelIndex);
    assert(load.playerPosition.x == saved.playerPosition.x && load.playerPosition.y == saved.playerPosition.y);
    
    std::remove("test_save.txt");
    assert(!hasSaveGame("test_save.txt"));
} 
