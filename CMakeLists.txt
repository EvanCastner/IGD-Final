cmake_minimum_required(VERSION 3.28.3 FATAL_ERROR)

project(GameEngine)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SDL2 REQUIRED)

file(GLOB_RECURSE ENGINE_SOURCE
    ${CMAKE_CURRENT_LIST_DIR}/src/*.cpp
)

add_library(engine STATIC ${ENGINE_SOURCE})

target_include_directories(engine PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/src
    ${SDL2_INCLUDE_DIRS}
)

# Main executable
add_executable(GameEngine
    ${CMAKE_CURRENT_LIST_DIR}/src/main.cpp
)

target_link_libraries(GameEngine PRIVATE 
    engine
    ${SDL2_LIBRARIES}
)
