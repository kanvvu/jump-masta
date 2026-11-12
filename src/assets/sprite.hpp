#pragma once
#include "raylib.h"
#include <vector>


class Sprite
{
private:
    const Texture2D* texture;

    Rectangle sourceRectangle;

    Vector2 origin;
    int rotation;
    Vector2 size;
    

public:
    Sprite(const Texture2D* texture, Rectangle sourceRectangle, Vector2 size = {64,64});
    ~Sprite() = default;

    Vector2 getSize() const {return size;};
    void draw(Vector2 position) const;
    void draw(Vector2 position, Vector2 size) const;
};

struct SpriteGroup {
    std::vector<Sprite> sprites;
    int xCount = 0;
    int yCount = 0;

    void draw(Vector2 position) const;
    void draw(Vector2 position, Vector2 size) const;
    void draw(Vector2 position, Vector2 size, Vector2 drawBounds) const;
};
