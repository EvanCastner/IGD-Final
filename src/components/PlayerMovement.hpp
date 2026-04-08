#pragma once

#include "../core/Component.hpp"
#include "../core/Entity.hpp"
#include "Transform.hpp"
#include <SDL2/SDL.h>

class PlayerMovement : public Component
{
public:
    float speed = 200.0f;

    void Update(float deltaTime) override
    {
        const Uint8 *keys = SDL_GetKeyboardState(NULL);

        Transform *transform = owner->GetComponent<Transform>();
        if (!transform)
            return;

        if (keys[SDL_SCANCODE_W])
            transform->y -= speed * deltaTime;
        if (keys[SDL_SCANCODE_S])
            transform->y += speed * deltaTime;
        if (keys[SDL_SCANCODE_A])
            transform->x -= speed * deltaTime;
        if (keys[SDL_SCANCODE_D])
            transform->x += speed * deltaTime;
    }
};