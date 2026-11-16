#pragma once
#include <vector>
#include <unordered_map>
#include "sprite.hpp"
#include "app/common.hpp"

struct PlayerAnimations {
    Animation idle;
    Animation aiming;
    Animation jumping;
    Animation falling;
};

struct PlayerSounds {
    Sound jump;
    Sound land;
    Sound bump;
};

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

    SpriteGroup background;
    PlayerAnimations playerAnimations;

    PlayerSounds playerSounds;
    
public:
    Assets();
    ~Assets();

    int getSpritesSize() {return static_cast<int>(sprites.size());};
    Sprite getSprite(int index);
    int getTextureWidth() {return texture.width/tileSize;};
    int getTextureHeight() {return texture.height/tileSize;};

    Sprite getGrass() {return sprites.at(22*4 + 6);};
    Sprite getWall() {return sprites.at(2);};
    Sprite getBackground() {return sprites.at(22*16 + 4);};

    const SpriteGroup& getBackgroundGroup() const {return background;};
    const PlayerAnimations& getPlayerAnimations() const {return playerAnimations;};
    const PlayerSounds& getPlayerSounds() const {return playerSounds;};


    std::vector<int> getSpritesInArea(Rectangle area);

    Assets(const Assets&) = delete;
    Assets& operator=(const Assets&) = delete;
};


