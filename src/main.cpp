// main.cpp
// PURPOSE  : Entry point. Initialises SDL2, opens a window,
//            runs a basic event loop, then shuts down cleanly.
// INTERACTS WITH: engine.hpp, SDL2 (external)

#include "engine/Engine.hpp"

int main()
{
  Engine engine;
  engine.Run();
  return 0;
}
