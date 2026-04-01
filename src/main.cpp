// main.cpp
// PURPOSE  : Entry point. Initialises SDL2, opens a window,
//            runs a basic event loop, then shuts down cleanly.
// INTERACTS WITH: engine.hpp, SDL2 (external)

#include "engine.hpp"
#include <SDL2/SDL.h>
#include <iostream>

int main(int argc, char* argv[]) {

  Initialise();

  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    std::cerr << "SDL_Init failed: " << SDL_GetError() << "\n";
    return 1;
  }

  SDL_Window* window = SDL_CreateWindow(
    "GameEngine",
    SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
    1280, 720,
    SDL_WINDOW_SHOWN
  );

  if (!window) {
    std::cerr << "SDL_CreateWindow Failed: " << SDL_GetError() << "\n";
    SDL_Quit();
    return 1;
  }

  std::cout << "Window Created Successfully\n";

  bool running = true;
  SDL_Event event;

  while(running) {
    while(SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT)
        running = false;

        if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)
          running = false;
    }
  }

  SDL_DestroyWindow(window);
  SDL_Quit();
  return 0;
}
