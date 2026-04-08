// FILE     : components/Transform.hpp
// PURPOSE  : Stores the 2D world position of an Entity.
//            Other components (e.g. PlayerMovement) read and
//            write x/y to move entities around the scene.
// DEPENDS  : core/Component.hpp

#pragma once

#include "../core/Component.hpp"

class Transform : public Component
{
public:
    float x = 0.0f;
    float y = 0.0f;
};
