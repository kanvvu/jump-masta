#include "sprite.hpp"

Sprite::Sprite(const Texture2D* texture, Rectangle sourceRectangle, Vector2 size)
: texture(texture),
  sourceRectangle(sourceRectangle),
  origin({0,0}),
  rotation(0),
  size(size)
{
}

void Sprite::draw(Vector2 position) const
{
    DrawTexturePro(*texture, sourceRectangle, {position.x, position.y, size.x, size.y}, origin, (float)rotation, WHITE);
}

void Sprite::draw(Vector2 position, Vector2 drawSize, bool flipX) const
{
    Rectangle drawSourceRectangle = {sourceRectangle.x, sourceRectangle.y, (flipX ? -sourceRectangle.width : sourceRectangle.width ), sourceRectangle.height};
    DrawTexturePro(*texture, drawSourceRectangle, {position.x, position.y, drawSize.x, drawSize.y}, origin, (float)rotation, WHITE);
}

void SpriteGroup::draw(Vector2 position) const
{
    for(int i=0; i<yCount; i++) {
        for( int j=0; j<xCount; j++) {
            int index = i*xCount + j;
            const Sprite& sprite = sprites[index];
            Vector2 newPosition = {position.x + j*sprite.getSize().x, position.y + i*sprite.getSize().y};
            sprite.draw(newPosition);
        }
    }
}

void SpriteGroup::draw(Vector2 position, Vector2 size) const
{
    for(int j=0; j<yCount; j++) {
        for( int i=0; i<xCount; i++) {
            int index = j*xCount + i;
            const Sprite& sprite = sprites[index];
            Vector2 newPosition = {position.x + i*size.x, position.y + j*size.y};
            sprite.draw(newPosition, size);
        }
    }
}

void SpriteGroup::draw(Vector2 position, Vector2 size, Vector2 drawBounds) const
{
    for(int j=0; j<yCount; j++) {
        for( int i=0; i<xCount; i++) {
            int index = j*xCount + i;
            const Sprite& sprite = sprites[index];
            Vector2 newPosition = {position.x + i*size.x, position.y + j*size.y};
            if (newPosition.x < drawBounds.x && newPosition.y < drawBounds.y)
                sprite.draw(newPosition, size);
        }
    }
}

void Animation::update(float dt)
{
    if (frames.empty()) return;

    timer += dt;
    if (timer >= frameDuration) {
        timer -= frameDuration;
        currentFrame = (currentFrame + 1) % frames.size(); 
    }
}

void Animation::reset()
{
    timer = 0.0f;
    currentFrame = 0;
}