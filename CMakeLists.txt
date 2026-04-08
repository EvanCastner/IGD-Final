cmake_minimum_required(VERSION 3.28.3 FATAL_ERROR)

project(GameEngine)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# SDL2
find_package(SDL2 REQUIRED)

# IMGUI
set(IMGUI_DIR ${CMAKE_CURRENT_LIST_DIR}/libs/imgui)

set(IMGUI_SOURCES
    ${IMGUI_DIR}/imgui.cpp
    ${IMGUI_DIR}/imgui_draw.cpp
    ${IMGUI_DIR}/imgui_tables.cpp
    ${IMGUI_DIR}/imgui_widgets.cpp
    ${IMGUI_DIR}/backends/imgui_impl_sdl2.cpp
    ${IMGUI_DIR}/backends/imgui_impl_sdlrenderer2.cpp
)

add_library(imgui STATIC ${IMGUI_SOURCES})

target_include_directories(imgui PUBLIC
    ${IMGUI_DIR}
    ${IMGUI_DIR}/backends
    ${SDL2_INCLUDE_DIRS}
)

target_link_libraries(imgui PUBLIC ${SDL2_LIBRARIES})

file(GLOB_RECURSE ENGINE_SOURCE
    ${CMAKE_CURRENT_LIST_DIR}/src/*.cpp
)

list(REMOVE_ITEM ENGINE_SOURCE ${CMAKE_CURRENT_LIST_DIR}/src/main.cpp)

add_library(engine STATIC ${ENGINE_SOURCE})

target_include_directories(engine PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/src
    ${SDL2_INCLUDE_DIRS}
    ${IMGUI_DIR}
    ${IMGUI_DIR}/backends
)

target_link_libraries(engine PUBLIC
    imgui
    ${SDL2_LIBRARIES}
)

# Main executable
add_executable(GameEngine
    ${CMAKE_CURRENT_LIST_DIR}/src/main.cpp
)

target_link_libraries(GameEngine PRIVATE 
    engine
    ${SDL2_LIBRARIES}
)

add_custom_target(run
    COMMAND GameEngine
    DEPENDS GameEngine
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    COMMENT "Building and running GameEngine..."
)
