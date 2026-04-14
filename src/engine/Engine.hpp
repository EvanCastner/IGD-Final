// FILE     : engine/Engine.hpp
// PURPOSE  : Core engine class. Manages the main loop, window
//            lifecycle, event handling, and entity storage.
// DEPENDS  : core/Entity.hpp, SDL2 (external)

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
    bool CheckCollision(Entity &a, Entity &b);
    void Render();
    void Shutdown();

    bool running;
    bool colliding = false;
    SDL_Window *window;
    SDL_Renderer *renderer;
    int selectedEntity;

    std::vector<Entity> entities;
};
