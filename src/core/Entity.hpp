#pragma once

#include "Component.hpp"
#include <vector>
#include <memory>

class Entity
{
private:
    std::vector<std::unique_ptr<Component>> components;

public:
    template <typename T, typename... Args>
    T *GetComponent(Args &&...args)
    {
        T *component = new T(std::forward<Args>(args)...);
        component->owner = this;
        components.emplace_back(component);
        return component;
    }

    void Update(float deltaTime)
    {
        for (auto &comp : components)
        {
            comp->Update(deltaTime);
        }
    }
};
