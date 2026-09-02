#pragma once

#include "interfaces.hpp"
#include "common.hpp"


class LevelEditor
{
private:
public:
    LevelEditor() = default;
    ~LevelEditor() = default;

    void proccessInput(GameState& gameState);
    void update();
    void draw();
};

