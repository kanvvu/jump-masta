#include "sprite.hpp"

Sprite::Sprite(const Texture2D* texture, Rectangle sourceRectangle, Vector2 size)
: texture(texture),
  sourceRectangle(sourceRectangle),
  origin({0,0}),
  rotation(0),
  size(size)
{
}

void Sprite::draw(Vector2 position)
{
    DrawTexturePro(*texture, sourceRectangle, {position.x, position.y, size.x, size.y}, origin, (float)rotation, WHITE);
}

void Sprite::draw(Vector2 position, Vector2 drawSize)
{
    DrawTexturePro(*texture, sourceRectangle, {position.x, position.y, drawSize.x, drawSize.y}, origin, (float)rotation, WHITE);
}