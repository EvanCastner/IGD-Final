// FILE     : runtime/Runtime.hpp
// PURPOSE  : Runs the game simulation from a captured SceneData.
//            Handles gravity, input, collision resolution, and
//            win/lose conditions. Completely separate from the editor.

#pragma once

#include <SDL2/SDL.h>
#include <vector>
#include "SceneData.hpp"

enum class RuntimeState
{
    Running,
    Won,
    Lost
};

struct RuntimeEntity
{
    EntityData data;
    float x, y;
    float vx = 0.0f;
    float vy = 0.0f;
    bool onGround = false;
};

class Runtime
{
public:
    Runtime(SDL_Renderer *renderer, SceneData scene);
    void Update(float deltaTime);
    void Render(float panelWidth, float menuHeight);
    void HandleInput(SDL_Event &event);
    bool isFinished() const { return state != RuntimeState::Running; }
    RuntimeState GetState() const { return state; }

private:
    SDL_Renderer *renderer;
    std::vector<RuntimeEntity> entities;
    RuntimeState state = RuntimeState::Running;
    RuntimeEntity *player = nullptr;

    void ResolveCollisions();
};