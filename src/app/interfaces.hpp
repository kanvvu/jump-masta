#pragma once

class IDraw
{
public:
    virtual ~IDraw() = default;

    virtual void draw() = 0;
};

class IUpdate
{
public:
    virtual ~IUpdate() = default;

    virtual void update(float elapsedTime) = 0;
};

class IScene
{
    public:
    virtual ~IScene() = default;

    virtual void processInput() = 0;
    virtual void update(float dt) = 0;
    virtual void draw() = 0;
};
