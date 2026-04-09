// FILE     : engine/Engine.cpp
// PURPOSE  : Implementation of the core Engine class. Handles
//            SDL2 initialisation, the main loop, event polling,
//            entity updates, and clean shutdown.
// DEPENDS  : engine/Engine.hpp, components/Transform.hpp,
//            components/PlayerMovement.hpp, SDL2 (external)

#include "Engine.hpp"
#include "../components/Transform.hpp"
#include "../components/PlayerMovement.hpp"
#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_sdlrenderer2.h"
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

    entities.reserve(100);

    entities.emplace_back();
    Entity &player = entities.back();

    player.AddComponent<Transform>();
    player.AddComponent<PlayerMovement>();

    std::cout << "Engine Initialised\n";
}

void Engine::HandleEvents()
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT)
            running = false;

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

// Render - clear screen, draw ImGui panels, present
void Engine::Render()
{

    ImGui_ImplSDLRenderer2_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();

    // Get window size
    int w, h;
    SDL_GetWindowSize(window, &w, &h);

    // Menu Bar
    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("Quit"))
                running = false;
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Edit"))
        {
            ImGui::MenuItem("Settiings (coming soon)", nullptr, false, false);
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }

    float panelWidth = 220.0f;
    float panelHeight = h - 20.0f;

    // Hierarchy Panel
    ImGui::SetNextWindowPos(ImVec2(0, 20), ImGuiCond_Always);
    ImGui::SetWindowSize(ImVec2(panelWidth, panelHeight), ImGuiCond_Always);
    ImGui::Begin("Heirarchy", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);

    for (int i = 0; i < (int)entities.size(); i++)
    {
        bool selected = (selectedEntity == i);
        if (ImGui::Selectable(entities[i].name.c_str(), selected))
            selectedEntity = i;
    }

    ImGui::End();

    // Inspector Panel
    ImGui::SetNextWindowPos(ImVec2(0, 20), ImGuiCond_Always);
    ImGui::SetWindowSize(ImVec2(220, 770), ImGuiCond_Always);
    ImGui::Begin("Inspector", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);

    if (selectedEntity >= 0 && selectedEntity < (int)entities.size())
    {
        Entity &entity = entities[selectedEntity];
        ImGui::Text("Name: %s", entity.name.c_str());
        ImGui::Separator();

        Transform *transform = entity.GetComponent<Transform>();
        if (transform)
        {
            ImGui::Text("Transform");
            ImGui::DragFloat("X", &transform->x, 1.0f);
            ImGui::DragFloat("Y", &transform->y, 1.0f);
        }
    }
    else
    {
        ImGui::Text("No entity selected");
    }

    ImGui::End();

    // Render
    ImGui::Render();
    SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255);
    SDL_RenderClear(renderer);
    ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData(), renderer);
    SDL_RenderPresent(renderer);

    std::cout << "ImGui frame started\n";
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
        Render();
    }
}

void Engine::Shutdown()
{
    if (window)
    {
        SDL_DestroyWindow(window);
        window = nullptr;
    }
    SDL_Quit();
}
