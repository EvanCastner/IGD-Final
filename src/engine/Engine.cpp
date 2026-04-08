#include "Engine.hpp"
#include "../components/Transform.hpp"
#include "../components/PlayerMovement.hpp"
#include <iostream>

Engine::Engine() : running(false), window(nullptr) {}

Engine::~Engine()
{
    Shutdown();
}

void Engine::Initialise()
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        std::cerr << "SDL_Init Failed: " << SDL_GetError() << std::endl;
        return;
    }

    window = SDL_CreateWindow(
        "GameEngine",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        1200, 720,
        SDL_WINDOW_SHOWN);

    if (!window)
    {
        std::cerr << "Window creation failed: " << SDL_GetError() << std::endl;
        return;
    }

    running = true;

    Entity player;
    player.AddComponent<Transform>();
    player.AddComponent<PlayerMovement>();

    entities.push_back(std::move(player));

    std::cout << "Engine Initialised\n";
}

void Engine::HandleEvents()
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT)
            running == false;

        if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)
            running = false;
    }
}

void Engine::Update(float deltaTime)
{
    for (auto &entity : entities)
    {
        entity.Update(deltaTime);
    }
}

void Engine::Run()
{
    Initialise();

    Uint32 lastTime = SDL_GetTicks();

    while (running)
    {
        Uint32 currentTime = SDL_GetTicks();
        float deltaTime = (currentTime - lastTime) / 1000.0f;
        lastTime = currentTime;

        HandleEvents();
        Update(deltaTime);
    }
}

void Engine::Shutdown()
{
    if (window)
    {
        SDL_DestroyWindow(window);
    }
    SDL_QUIT;
}