#ifndef	MOVEMENT_COMPONENT_H__
#define MOVEMENT_COMPONENT_H__

#include <queue>

//Godot includes
#include <Vector2.hpp>

#include "cst.h"

using namespace godot;

struct MotorComponent
{
	Behaviour behaviour;
	std::queue<Vector2> path;
    Vector2 normalizedTarget;
	float baseSpeed;
	float curSpeed;
};

#endif