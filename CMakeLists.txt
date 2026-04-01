cmake_minimum_required(VERSION 3.28.3 FATAL_ERROR)

project(GameEngine)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SDL2 CONFIG REQUIRED)

set(ENGINE_SOURCE
    ${CMAKE_CURRENT_LIST_DIR}/src/engine.cpp
)
add_library(engine STATIC ${ENGINE_SOURCE})

target_include_directories(engine PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/src
)

# Main executable
add_executable(GameEngine
    ${CMAKE_CURRENT_LIST_DIR}/src/main.cpp
)

target_link_libraries(GameEngine PRIVATE 
    engine
    SDL2::SDL2
    SDL2::SDL2main
)
