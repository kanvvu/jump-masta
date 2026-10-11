#include "sprite.hpp"

Sprite::Sprite(Texture2D &texture, Rectangle sourceRectangle, Vector2 size)
: texture(texture),
  sourceRectangle(sourceRectangle),
  origin({0,0}),
  rotation(0),
  size(size)
{
}

void Sprite::draw(Vector2 position)
{
    DrawTexturePro(texture, sourceRectangle, {position.x, position.y, size.x, size.y}, origin, (float)rotation, WHITE);
}

void Sprite::draw(Vector2 position, Vector2 size)
{
    DrawTexturePro(texture, sourceRectangle, {position.x, position.y, size.x, size.y}, origin, (float)rotation, WHITE);
}