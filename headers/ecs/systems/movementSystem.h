#ifndef MOVEMENT_SYSTEM_H__
#define MOVEMENT_SYSTEM_H__

#include <iostream>
#include <math.h>

//components
#include "motorComponent.h"
#include "transformComponent.h"

//required
#include "basicLib.h"
#include "checkpointList.h"
#include "chain.h"

class MovementSystem
{
public:
	MovementSystem();
	MovementSystem(CheckpointList<MotorComponent>& motors, CheckpointList<TransformComponent>& transforms);
	~MovementSystem();
	void Loop(const float& deltaTime);
private:
	void SwitchBehaviour(const float& deltaTime, MotorComponent& motor, TransformComponent& transforms);
	void MoveMotor(const float& deltaTime, MotorComponent& motor, TransformComponent& transforms);
private:
	CheckpointList<MotorComponent>* m_motors;
	CheckpointList<TransformComponent>* m_transforms;
};

#endif