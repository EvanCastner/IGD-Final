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
        SDL_WINDOW_SHOWN | SDL_WINDOW_FULLSCREEN_DESKTOP);

    if (!window)
    {
        std::cerr << "Window creation failed: " << SDL_GetError() << std::endl;
        return;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer)
    {
        std::cerr << "Renderer creation failed: " << SDL_GetError() << std::endl;
        return;
    }

    // ImGui Setup
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();
    ImGui_ImplSDL2_InitForSDLRenderer(window, renderer);
    ImGui_ImplSDLRenderer2_Init(renderer);

    running = true;
    entities.reserve(100);
    entities.emplace_back();

    Entity &player = entities.back();
    player.name = "Player";
    player.AddComponent<Transform>();
    player.AddComponent<PlayerMovement>();

    std::cout << "Engine Initialised\n";
}

void Engine::HandleEvents()
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        ImGui_ImplSDL2_ProcessEvent(&event);

        if (runtime)
            runtime->HandleInput(event);

        if (event.type == SDL_QUIT)
            running = false;

        if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)
            running = false;

        // Click in viewport to place selected entity
        if (!runtime && event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT)
        {
            float panelWidth = 300.0f;
            float menuHeight = ImGui::GetFrameHeight();
            float W = ImGui::GetIO().DisplaySize.x;

            int mx = event.button.x;
            int my = event.button.y;

            // Only if click is inside the viewport
            bool inViewport = mx > panelWidth && mx < (W - panelWidth) && my > menuHeight;

            if (inViewport && selectedEntity >= 0 && selectedEntity < (int)entities.size())
            {
                Transform *t = entities[selectedEntity].GetComponent<Transform>();
                if (t)
                {
                    t->x = (float)(mx - panelWidth);
                    t->y = (float)(my - menuHeight);
                }
            }
        }
    }
}

void Engine::Update(float deltaTime)
{
    // Runtime update
    if (runtime)
    {
        runtime->Update(deltaTime);
        if (runtime->isFinished())
        {
            runtime.reset();
        }
        return;
    }

    // Editor update
    for (auto &entity : entities)
    {
        entity.Update(deltaTime);
    }

    // Reset collision state
    for (auto &entity : entities)
        entity.colliding = false;

    // Collision Detection
    for (int i = 0; i < (int)entities.size(); i++)
    {
        for (int j = i + 1; j < (int)entities.size(); j++)
        {
            if (entities[i].solid && entities[j].solid)
            {
                if (CheckCollision(entities[i], entities[j]))
                {
                    // Both set to true for now
                    entities[i].colliding = true;
                    entities[j].colliding = true;
                }
            }
        }
    }
}

// Render - clear screen, draw ImGui panels, present
void Engine::Render()
{

    ImGui_ImplSDLRenderer2_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();

    float W = ImGui::GetIO().DisplaySize.x;
    float H = ImGui::GetIO().DisplaySize.y;
    float menuHeight = ImGui::GetFrameHeight();
    float panelWidth = 300.0f;
    float panelHeight = H - menuHeight;

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
        if (!runtime) 
        {
            if (ImGui::Button("Play"))
            {   
                runtime = std::make_unique<Runtime>(renderer, CaptureScene());
            }
        else 
        {
            if (ImGui::Button(" Stop "))
            {
                runtime.reset();
            }
        }
        }
        ImGui::EndMainMenuBar();
    }

    if (!runtime)
    {    
        // Hierarchy Panel
        ImGui::SetNextWindowPos(ImVec2(0, menuHeight), ImGuiCond_Always);
        ImGui::SetNextWindowSize(ImVec2(panelWidth, panelHeight), ImGuiCond_Always);
        ImGui::Begin("Heirarchy", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);

        // Add entity button
        if (ImGui::Button("Add Entity", ImVec2(-1, 0)))
        {
            entities.emplace_back();
            Entity &newEntity = entities.back();
            newEntity.name = "Entity: " + std::to_string(entities.size() - 1);
            newEntity.AddComponent<Transform>();
            selectedEntity = (int)entities.size() - 1;
        }

        ImGui::Separator();

        // Entity List
        for (int i = 0; i < (int)entities.size(); i++)
        {
            bool selected = (selectedEntity == i);
            std::string displayName = entities[i].name.empty() ? "(unnamed)" : entities[i].name;
            if (ImGui::Selectable(displayName.c_str(), selected))
                selectedEntity = i;
        }

        // Remove entity button
        if (selectedEntity >= 0 && selectedEntity < (int)entities.size())
        {
            ImGui::Separator();
            if (ImGui::Button("Remove Entity", ImVec2(-1, 0)))
            {
                entities.erase(entities.begin() + selectedEntity);
                selectedEntity = -1;
            }
        }

        ImGui::End();
    }

    if (!runtime)
    {
        // Inspector Panel
        ImGui::SetNextWindowPos(ImVec2(W - panelWidth, menuHeight), ImGuiCond_Always);
        ImGui::SetNextWindowSize(ImVec2(panelWidth, panelHeight), ImGuiCond_Always);
        ImGui::Begin("Inspector", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);

        if (selectedEntity >= 0 && selectedEntity < (int)entities.size())
        {
            Entity &entity = entities[selectedEntity];
            // ImGui::Text("Name: %s", entity.name.c_str());
            char nameBuf[64];
            strncpy(nameBuf, entity.name.c_str(), sizeof(nameBuf));
            nameBuf[sizeof(nameBuf) - 1] = '\0';
            if (ImGui::InputText("##name", nameBuf, sizeof(nameBuf)))
                entity.name = nameBuf;
            ImGui::Separator();

            // Shape Selector
            ImGui::Text("Shape");
            if (ImGui::RadioButton("Rectangle", entity.shape == Shape::Rectangle))
                entity.shape = Shape::Rectangle;
            ImGui::SameLine();
            if (ImGui::RadioButton("Square", entity.shape == Shape::Square))
                entity.shape = Shape::Square;
            ImGui::SameLine();
            if (ImGui::RadioButton("Circle", entity.shape == Shape::Circle))
                entity.shape = Shape::Circle;

            ImGui::Separator();

            Transform *transform = entity.GetComponent<Transform>();
            if (transform)
            {
                ImGui::Text("Transform");
                ImGui::DragFloat("X", &transform->x, 1.0f);
                ImGui::DragFloat("Y", &transform->y, 1.0f);
            }

            ImGui::Separator();
            ImGui::Text("Size");
            if (entity.shape == Shape::Rectangle)
            {
                ImGui::DragFloat("Width", &entity.width, 1.0f, 1.0f, 1000.0f);
                ImGui::DragFloat("Height", &entity.height, 1.0f, 1.0f, 1000.0f);
            }
            else if (entity.shape == Shape::Square)
            {
                ImGui::DragFloat("Size", &entity.width, 1.0f, 1.0f, 1000.0f);
                entity.height = entity.width;
            }
            else if (entity.shape == Shape::Circle)
            {
                ImGui::DragFloat("Radius", &entity.radius, 1.0f, 1.0f, 500.0f);
            }

            ImGui::Separator();
            ImGui::Text("Color");
            float color[3] = {entity.colorR, entity.colorG, entity.colorB};
            if (ImGui::ColorEdit3("##color", color))
            {
                entity.colorR = color[0];
                entity.colorG = color[1];
                entity.colorB = color[2];
            }

            ImGui::Separator();
            ImGui::Checkbox("Solid", &entity.solid);
            ImGui::Checkbox("Goal", &entity.isGoal);
        }
        else
        {
            ImGui::Text("No entity selected");
        }

        ImGui::End();
    }
    

    // Render
    ImGui::Render();
    SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255);
    SDL_RenderClear(renderer);

    if (runtime)
    {
        runtime->Render(panelWidth, menuHeight);
    }
    else 
    {
        // Draw all entites that have a transform
        for (auto &entity : entities)
        {
            Transform *transform = entity.GetComponent<Transform>();
            if (!transform)
                continue;

            int x = (int)(panelWidth + transform->x);
            int y = (int)(menuHeight + transform->y);

            if (entity.colliding)
            {
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            }
            else
            {
                SDL_SetRenderDrawColor(renderer,
                                    (Uint8)(entity.colorR * 255),
                                    (Uint8)(entity.colorG * 255),
                                    (Uint8)(entity.colorB * 255),
                                    255);
            }

            if (entity.shape == Shape::Rectangle)
            {
                SDL_Rect rect = {x, y, (int)entity.width, (int)entity.height};
                SDL_RenderFillRect(renderer, &rect);
            }
            else if (entity.shape == Shape::Square)
            {
                SDL_Rect rect = {x, y, (int)entity.width, (int)entity.width};
                SDL_RenderFillRect(renderer, &rect);
            }
            else if (entity.shape == Shape::Circle)
            {
                // Build circle with points since SDL Does not have one
                int radius = (int)entity.radius;
                for (int dy = -radius; dy <= radius; dy++)
                {
                    int dx = (int)sqrt((double)(radius * radius - dy * dy));
                    SDL_RenderDrawLine(renderer,
                                    x - dx, y + dy,
                                    x + dx, y + dy);
                }
            }
        }
    }
    ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData(), renderer);
    SDL_RenderPresent(renderer);
}

// Collision Detection
bool Engine::CheckCollision(Entity &a, Entity &b)
{
    Transform *ta = a.GetComponent<Transform>();
    Transform *tb = b.GetComponent<Transform>();
    if (!ta || !tb)
        return false;

    float aLeft = ta->x;
    float aRight = ta->x + a.width;
    float aTop = ta->y;
    float aBottom = ta->y + a.height;

    float bLeft = tb->x;
    float bRight = tb->x + b.width;
    float bTop = tb->y;
    float bBottom = tb->y + b.height;

    return aLeft < bRight && aRight > bLeft && aTop < bBottom && aBottom > bTop;
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

SceneData Engine::CaptureScene()
{
    SceneData data;
    for (auto &entity : entities)
    {
        Transform *t = entity.GetComponent<Transform>();
        if (!t)
            continue;

        EntityData ed;
        ed.name = entity.name;
        ed.x = t->x;
        ed.y = t->y;
        ed.width = entity.width;
        ed.height = entity.height;
        ed.radius = entity.radius;
        ed.colorR = entity.colorR;
        ed.colorG = entity.colorG;
        ed.colorB = entity.colorB;
        ed.shape = entity.shape;
        ed.solid = entity.solid;
        ed.isGoal = entity.isGoal;
        ed.hasPlayerMovement = entity.GetComponent<PlayerMovement>() != nullptr;
        data.entities.push_back(ed);
    }

    return data;
}
