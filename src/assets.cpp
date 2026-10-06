#include "assets.hpp"
#include <iostream>

Assets::Assets() : 
 texture(LoadTexture("resources/map.png"))
{
    sprites.push_back(Sprite(texture, {0,0,32,32})); // Player
    sprites.push_back(Sprite(texture, {96,64,32,32})); // Grass
    sprites.push_back(Sprite(texture, {96,80,32,32})); // Wall
    sprites.push_back(Sprite(texture, {0,240,64,64}, {128,128})); // Background
    sprites.push_back(Sprite(texture, {112,80,32,32})); // Wall2

    spritesMap.insert({PLAYER, Sprite(texture, {0,0,32,32})});
}

Assets::~Assets()
{
    UnloadTexture(texture);
}

Sprite Assets::getSprite(int index)
{
    if (index < 0 || index >= sprites.size()) {
        std::cout<<"Error [Assets::getSprite] wrong index: "<<index<<"\n";
    }

    return sprites.at(index);

}
