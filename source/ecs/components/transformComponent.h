#ifndef TRANSFORM_COMPONENT_H__
#define TRANSFORM_COMPONENT_H__

#include "vector2.h"

struct TransformComponent
{
    Vector2 position;
    Vector2 normalizedTarget;
    //Vector3 scale;
};

#endif