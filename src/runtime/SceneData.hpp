// FILE     : runtime/SceneData.hpp
// PURPOSE  : Plain data structures for serializing the editor
//            scene into a format the Runtime can consume.
//            No SDL, no ImGui, no engine dependencies.

#pragma once

#include <string>
#include <vector>
#include "core/Entity.hpp"

struct EntityData
{
    std::string name;
    float x = 0.0f;
    float y = 0.0f;
    float width = 80.0f;
    float height = 40.0f;
    float radius = 20.0f;
    float colorR = 1.0f;
    float colorG = 0.39f;
    float colorB = 0.39f;

    Shape shape = Shape::Rectangle;

    bool solid = false;
    bool isGoal = false;
    bool hasPlayerMovement = false;
};

struct SceneData
{
    std::vector<EntityData> entities;
};
