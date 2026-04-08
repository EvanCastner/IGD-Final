#pragma once

#include "Component.hpp"
#include <vector>
#include <memory>

class Entity
{
private:
    std::vector<std::unique_ptr<Component>> components;

public:
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
