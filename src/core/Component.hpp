// FILE     : core/Component.hpp
// PURPOSE  : Abstract base class for all components. Provides
//            a pointer back to the owning Entity and virtual
//            lifecycle hooks (Start, Update) for subclasses.
// DEPENDS  : core/Entity.hpp (forward declared)

#pragma once

class Entity; // Forward Declaration

class Component
{
public:
    Entity *owner = nullptr;

    virtual void Start() {}
    virtual void Update(float deltaTime) {}
    virtual ~Component() {}
};
