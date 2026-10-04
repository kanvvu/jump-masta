#include "sprite.hpp"

Sprite::Sprite(Texture2D &texture, Rectangle sourceRectangle)
: texture(texture),
  sourceRectangle(sourceRectangle),
  origin({0,0}),
  rotation(0)
{
}

void Sprite::draw(Vector2 position)
{
    DrawTexturePro(texture, sourceRectangle, {position.x, position.y, 64, 64}, origin, (float)rotation, WHITE);
}