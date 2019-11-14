#ifndef	MOVEMENT_COMPONENT_H__
#define MOVEMENT_COMPONENT_H__

//Godot includes
#include "Vector2.hpp"

#include "chain.h"
#include "cst.h"

struct MotorComponent
{
	Behaviour behaviour;
	Chain<godot::Vector2> path;
	float baseSpeed;
	float curSpeed;
};

#endif