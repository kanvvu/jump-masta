#pragma once
#include "raylib.h"

class Sprite
{
private:
    Texture2D& texture;

    Rectangle sourceRectangle;

    Vector2 origin;
    int rotation;
    Vector2 size;
    

public:
    Sprite(Texture2D& texture, Rectangle sourceRectangle, Vector2 size = {64,64});
    ~Sprite() = default;

    Vector2 getSize() {return size;};
    void draw(Vector2 position);
};

