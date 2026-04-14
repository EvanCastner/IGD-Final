// FILE     : core/Entity.hpp
// PURPOSE  : Represents a game object as a container of
//            Components. Supports adding, querying, and ticking
//            components each frame via Update().
// DEPENDS  : core/Component.hpp

#pragma once

#include "Component.hpp"
#include <vector>
#include <memory>
#include <string>

enum class Shape
{
    Rectangle,
    Square,
    Circle
};

class Entity
{
private:
    std::vector<std::unique_ptr<Component>> components;

public:
    Shape shape = Shape::Rectangle;

    std::string name;
    float width = 80.0f;
    float height = 40.0f;
    float radius = 20.0f;

    // Prevent copying and allow moving
    Entity() = default;
    Entity(const Entity &) = delete;
    Entity &operator=(const Entity &) = delete;
    Entity(Entity &&) = default;
    Entity &operator=(Entity &&) = default;

    template <typename T, typename... Args>
    T *AddComponent(Args &&...args)
    {
        auto comp = std::make_unique<T>(std::forward<Args>(args)...);
        comp->owner = this;

        T *ptr = comp.get();
        components.emplace_back(std::move(comp));

        return ptr;
    }

    template <typename T>
    T *GetComponent()
    {
        for (auto &comp : components)
        {
            if (!comp)
                continue;
            T *casted = dynamic_cast<T *>(comp.get());
            if (casted)
                return casted;
        }
        return nullptr;
    }

    void Update(float deltaTime)
    {
        for (auto &comp : components)
        {
            if (!comp)
                continue;
            comp->Update(deltaTime);
        }
    }
};
