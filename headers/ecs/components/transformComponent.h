#ifndef TRANSFORM_COMPONENT_H__
#define TRANSFORM_COMPONENT_H__

//Godot includes
#include "Vector2.hpp"

struct TransformComponent
{
    godot::Vector2 position;
    godot::Vector2 normalizedTarget;
    //Vector3 scale;
};

#endif