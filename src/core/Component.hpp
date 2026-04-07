#pragma once

class Entity; // Forward Declaration

class Component
{
public:
    Entity *owner;

    virtual void Start() {}
    virtual void Update(float deltaTime) {}
    virtual ~Component() {}
};
