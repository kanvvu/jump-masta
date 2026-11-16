#include "assets.hpp"
#include <iostream>

Assets::Assets() : 
 texture(LoadTexture("resources/map.png")) 
{
    int width = texture.width / tileSize;
    int height = texture.height / tileSize;
    for(int i=0; i<height; i++) {
        for(int j=0; j<width; j++) {
            sprites.push_back(Sprite(&texture, {static_cast<float>(j*tileSize), static_cast<float>(i*tileSize), static_cast<float>(tileSize), static_cast<float>(tileSize)}));
        }
    }

    std::vector<int> backgroundIDs = {
        364, 365, 366, 367,
        386, 387, 388, 389,
        408, 409, 410, 411,
        430, 431, 432, 433
    };

    
    background.xCount = 4;
    background.yCount = 4;
    for(const auto& i : backgroundIDs) {
        background.sprites.push_back(getSprite(i));
    }
    for(int i=0; i<10; i++) {
        playerAnimations.idle.frames.push_back(Sprite(&texture, {i*32.0f,0.0f,32.0f,32.0f}));
    }
    playerAnimations.aiming.frames.push_back(Sprite(&texture, {80.0f, 32.0f, 32.0f, 32.0f}));
    playerAnimations.falling.frames.push_back(Sprite(&texture, {112.0f, 32.0f, 32.0f, 32.0f}));
    playerAnimations.jumping.frames.push_back(Sprite(&texture, {0.0f, 32.0f, 32.0f, 32.0f}));

    playerSounds.bump = LoadSound("resources/bump.wav");
    playerSounds.jump = LoadSound("resources/jump.wav");
    playerSounds.land = LoadSound("resources/land.wav");
}

Assets::~Assets()
{
    UnloadTexture(texture);
    UnloadSound(playerSounds.bump);
    UnloadSound(playerSounds.jump);
    UnloadSound(playerSounds.land);
}

Sprite Assets::getSprite(int index)
{
    if (index < 0 || index >= sprites.size()) {
        std::cout<<"Error [Assets::getSprite] wrong index: "<<index<<"\n";
    }

    return sprites.at(index);

}
