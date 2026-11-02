#pragma once
#include <vector>
#include <unordered_map>
#include "sprite.hpp"


enum class SpriteNames {
    PLAYER = 1,
    GRASS,
    WALL,
    BACKGROUND
};

class Assets
{
private:
    Texture2D texture;
    int tileSize = 16;

    std::vector<Sprite> sprites;
    std::unordered_map<int, Sprite> spritesMap;

    Sprite player;

    
public:
    Assets();
    ~Assets();

    int getSpritesSize() {return sprites.size();};
    Sprite getSprite(int index);
    int getTextureWidth() {return texture.width/tileSize;};
    int getTextureHeight() {return texture.height/tileSize;};

    // Sprite getPlayer() {return sprites.at(11);};
    Sprite getPlayer() {return player;};
    Sprite getGrass() {return sprites.at(22*4 + 6);};
    Sprite getWall() {return sprites.at(2);};
    Sprite getBackground() {return sprites.at(22*16 + 4);};

    std::vector<int> getSpritesInArea(Rectangle area);

    Assets(const Assets&) = delete;
    Assets& operator=(const Assets&) = delete;
};

