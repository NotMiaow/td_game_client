#ifndef	MOVEMENT_COMPONENT_H__
#define MOVEMENT_COMPONENT_H__

#include <deque>

#include <Vector2.hpp>

#include "cst.h"

using namespace godot;

struct MotorComponent
{
	MotorComponent() { }
	MotorComponent(const int& baseSpeed) : behaviour(WaitingForPath), baseSpeed(baseSpeed), curSpeed(baseSpeed) { }
	Behaviour behaviour;
	std::deque<Vector2> path;
    Vector2 normalizedTarget;
	float baseSpeed;
	float curSpeed;
};

#endif