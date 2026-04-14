// FILE     : runtime/Runtime.cpp
// PURPOSE  : Implementation of the Runtime class. Handles gravity,
//            player input, AABB collision resolution, and win condition.

#include "Runtime.hpp"
#include <SDL2/SDL.h>
#include <cmath>
#include <algorithm>
#include <iostream>

static const float GRAVITY = 1500.0f;
static const float JUMP_FORCE = -600.0f;
static const float MOVE_SPEED = 300.0f;

Runtime::Runtime(SDL_Renderer *renderer, SceneData scene)
    : renderer(renderer)
{
    for (auto &ed : scene.entities)
    {
        RuntimeEntity re;
        re.data = ed;
        re.x = ed.x;
        re.y = ed.y;
        entities.push_back(re);
    }

    // Find the player
    for (auto &re : entities)
    {
        if (re.data.hasPlayerMovement)
        {
            player = &re;
            break;
        }
    }
}

void Runtime::HandleInput(SDL_Event &event)
{
    if (!player)
        return;

    if (event.type == SDL_KEYDOWN)
    {
        if (event.key.keysym.sym == SDLK_SPACE || event.key.keysym.sym == SDLK_w)
        {
            if (player->onGround)
            {
                player->vy = JUMP_FORCE;
                player->onGround = false;
            }
        }
    }
}

void Runtime::Update(float deltaTime)
{
    if (!player)
        return;

    // Horizontal input
    const Uint8 *keys = SDL_GetKeyboardState(nullptr);
    if (keys[SDL_SCANCODE_A])
        player->vx = -MOVE_SPEED;
    else if (keys[SDL_SCANCODE_D])
        player->vx = MOVE_SPEED;
    else
        player->vx = 0.0f;

    // Apply gravity
    player->vy += GRAVITY * deltaTime;

    // Move player
    player->x += player->vx * deltaTime;
    player->y += player->vy * deltaTime;

    player->onGround = false;
    ResolveCollisions();
}

void Runtime::ResolveCollisions()
{
    if (!player)
        return;

    for (auto &re : entities)
    {
        if (&re == player)
            continue;
        if (!re.data.solid && !re.data.isGoal)
            continue;

        float pw = player->data.width;
        float ph = player->data.height;
        float ow = re.data.width;
        float oh = re.data.height;

        float pLeft = player->x;
        float pRight = player->x + pw;
        float pTop = player->y;
        float pBottom = player->y + ph;

        float oLeft = re.x;
        float oRight = re.x + ow;
        float oTop = re.y;
        float oBottom = re.y + oh;

        // Check overlap
        if (pRight <= oLeft || pLeft >= oRight ||
            pBottom <= oTop || pTop >= oBottom)
            continue;

        // Goal check
        if (re.data.isGoal)
        {
            state = RuntimeState::Won;
            return;
        }

        // Resolve by smallest overlap
        float overlapLeft = pRight - oLeft;
        float overlapRight = oRight - pLeft;
        float overlapTop = pBottom - oTop;
        float overlapBottom = oBottom - pTop;

        float minOverlap = std::min({overlapLeft, overlapRight, overlapTop, overlapBottom});

        if (minOverlap == overlapTop)
        {
            player->y = oTop - ph;
            player->vy = 0.0f;
            player->onGround = true;
        }
        else if (minOverlap == overlapBottom)
        {
            player->y = oBottom;
            player->vy = 0.0f;
        }
        else if (minOverlap == overlapLeft)
        {
            player->x = oLeft - pw;
            player->vx = 0.0f;
        }
        else if (minOverlap == overlapRight)
        {
            player->x = oRight;
            player->vx = 0.0f;
        }
    }
}

void Runtime::Render(float panelWidth, float menuHeight)
{
    for (auto &re : entities)
    {
        int x = (int)(panelWidth + re.x);
        int y = (int)(menuHeight + re.y);

        SDL_SetRenderDrawColor(renderer,
                               (Uint8)(re.data.colorR * 255),
                               (Uint8)(re.data.colorG * 255),
                               (Uint8)(re.data.colorB * 255),
                               255);

        if (re.data.shape == Shape::Rectangle || re.data.shape == Shape::Square)
        {
            SDL_Rect rect = {x, y, (int)re.data.width, (int)re.data.height};
            SDL_RenderFillRect(renderer, &rect);
        }
        else if (re.data.shape == Shape::Circle)
        {
            int radius = (int)re.data.radius;
            for (int dy = -radius; dy <= radius; dy++)
            {
                int dx = (int)sqrt((double)(radius * radius - dy * dy));
                SDL_RenderDrawLine(renderer, x - dx, y + dy, x + dx, y + dy);
            }
        }
    }

    // Win screen
    if (state == RuntimeState::Won)
    {
        SDL_SetRenderDrawColor(renderer, 0, 200, 0, 100);
        SDL_Rect overlay = {0, 0, 9999, 9999};
        SDL_RenderFillRect(renderer, &overlay);
    }
}
