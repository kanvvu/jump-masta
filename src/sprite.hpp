#pragma once
#include "raylib.h"

class Sprite
{
private:
    Texture2D& texture;

    Rectangle sourceRectangle;

    Vector2 origin;
    int rotation;
    

public:
    Sprite(Texture2D& texture, Rectangle sourceRectangle);
    ~Sprite() = default;

    void draw(Vector2 position);
};

