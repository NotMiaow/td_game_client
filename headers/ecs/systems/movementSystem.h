#ifndef MOVEMENT_SYSTEM_H__
#define MOVEMENT_SYSTEM_H__

#include <iostream>
#include <math.h>
#include <queue>

//Godot
#include <Godot.hpp>
#include <Node.hpp>
#include <RigidBody.hpp>

//components
#include "motorComponent.h"
#include "transformComponent.h"

//required
#include "basicLib.h"
#include "checkpointList.h"

class MovementSystem
{
public:
	MovementSystem();
	MovementSystem(Node* root, CheckpointList<MotorComponent>& motors, CheckpointList<TransformComponent>& transforms);
	~MovementSystem();
	void Loop(const float& deltaTime);
private:
	void MoveMotor(const float& deltaTime, const int& index, MotorComponent& motor, TransformComponent& transforms);
private:
	CheckpointList<MotorComponent>* m_motors;
	CheckpointList<TransformComponent>* m_transforms;

    Node* m_unitGroups;
    Node* m_projectiles;
};

#endif