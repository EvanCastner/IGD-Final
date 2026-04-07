#pragma once

#include "../core/Entity.hpp"
#include <SDL2/SDL.h>
#include <vector>

class Engine
{
public:
    Engine();
    ~Engine();

    void Run();

private:
    void Initialise();
    void HandleEvents();
    void Update(float deltaTime);
    void Shutdown();

    bool running;
    SDL_Window *window;
    std::vector<Entity> entities;
};