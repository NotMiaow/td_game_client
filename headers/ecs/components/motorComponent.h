#ifndef	MOVEMENT_COMPONENT_H__
#define MOVEMENT_COMPONENT_H__

//Godot includes
#include <Vector2.hpp>

#include "queue.h"
#include "cst.h"

using namespace godot;

struct MotorComponent
{
	Behaviour behaviour;
	Queue<Vector2> path;
    Vector2 normalizedTarget;
	float baseSpeed;
	float curSpeed;
};

#endif