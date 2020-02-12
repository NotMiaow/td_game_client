#ifndef TRANSFORM_COMPONENT_H__
#define TRANSFORM_COMPONENT_H__

#include <Vector2.hpp>

using namespace godot;

struct TransformComponent
{
    TransformComponent() { }
    TransformComponent(const Vector2& position) : position(position) { }

    Vector2 position;
    //Vector3 scale;
};

#endif