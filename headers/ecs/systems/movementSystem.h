#ifndef MOVEMENT_SYSTEM_H__
#define MOVEMENT_SYSTEM_H__

#include <iostream>
#include <math.h>
#include <deque>

#include "basicLib.h"

//Godot
#include <Godot.hpp>
#include <Node.hpp>
#include <RigidBody.hpp>
#include <Vector2.hpp>

#include "componentArray.h"
//components
#include "motorComponent.h"
#include "transformComponent.h"

//required
#include "definitions.h"

class MovementSystem
{
public:
	MovementSystem();
	MovementSystem(Node* root, Motors& motors, Transforms& transforms);
	~MovementSystem();
	void Loop(const float& deltaTime);
private:
	void MoveMotor(const float& deltaTime, const int& index, MotorComponent& motor, TransformComponent& transforms);
private:
	Motors* m_motors;
	Transforms* m_transforms;

    Node* m_unitGroups;
    Node* m_projectiles;
};

#endif