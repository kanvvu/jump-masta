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
