IGDEngine
A lightweight 2D game engine written in C++, built as a course project for Interactive Game Design.
Features

Core game loop with delta time
Entity Component System (ECS)
2D renderer (SDL2)
AABB collision detection & basic physics
AI behaviors (pathfinding, state machines)
Scene graph with parent-child transforms
Asset manager (textures, audio)

Getting Started
Prerequisites

C++17 or later
CMake 3.16+
SDL2 (fetched automatically via CMake)

Build
bashgit clone https://github.com/yourusername/IGDEngine.git
cd IGDEngine
mkdir build && cd build
cmake ..
make
Run the demo
bash./demo/IGDDemo
Project Structure
IGDEngine/
├── src/
│   ├── core/        # Game loop, engine entry point
│   ├── ecs/         # Entity Component System
│   ├── renderer/    # Rendering system
│   ├── physics/     # Collision detection
│   ├── ai/          # Pathfinding and behavior
│   └── assets/      # Asset loading and management
├── demo/            # Example game built with the engine
├── CMakeLists.txt
└── README.md
Demo
The included demo is a simple top-down game showcasing the engine's systems — an AI enemy that pathfinds toward the player using A*, with collision, sprites, and basic physics.

Screenshot or GIF here once the demo is built.

Built With

SDL2 — windowing, input, and 2D rendering
C++17

Author
Your Name — Interactive Game Design, Spring 2026
License
MIT