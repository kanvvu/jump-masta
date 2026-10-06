#pragma once
#include <vector>
#include <unordered_map>
#include "sprite.hpp"


enum SpriteNames {
    PLAYER = 1,
    GRASS,
    WALL,
    BACKGROUND
};

class Assets
{
private:
    Texture2D texture;

    std::vector<Sprite> sprites;
    std::unordered_map<int, Sprite> spritesMap;

    
public:
    Assets();
    ~Assets();

    int getSpritesSize() {return sprites.size();};
    Sprite getSprite(int index);

    Sprite getPlayer() {return sprites.at(0);};
    Sprite getGrass() {return sprites.at(1);};
    Sprite getWall() {return sprites.at(2);};
    Sprite getBackground() {return sprites.at(3);};

    Assets(const Assets&) = delete;
    Assets& operator=(const Assets&) = delete;
};

