#include "assets.hpp"

Assets::Assets() : 
 texture(LoadTexture("resources/map.png")),
 player(texture, {0,0,32,32}),
 grass(texture, {96,64,32,32}),
 wall(texture, {96,64,32,32}),
 background(texture, {0,240,64,64})
{
}

Assets::~Assets()
{
    UnloadTexture(texture);
}