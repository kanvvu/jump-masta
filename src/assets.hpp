#pragma once
#include "sprite.hpp"

class Assets
{
private:
    Texture2D texture;

    Sprite player;
    Sprite grass;
    Sprite wall;
    Sprite background;
    
public:
    Assets();
    ~Assets();

    Sprite getPlayer() {return player;};
    Sprite getGrass() {return grass;};
    Sprite getWall() {return wall;};
    Sprite getBackground() {return background;};

    Assets(const Assets&) = delete;
    Assets& operator=(const Assets&) = delete;
};

