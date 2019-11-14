#ifndef MOVEMENT_SYSTEM_H__
#define MOVEMENT_SYSTEM_H__

#include <iostream>
#include <math.h>

//Systems
#include "timeSystem.h"

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
	MovementSystem(TimeSystem& timeSystem, CheckpointList<MotorComponent>& motors, CheckpointList<TransformComponent>& transforms);
	~MovementSystem();
	void Loop();
private:
	void SwitchBehaviour(MotorComponent& motor, TransformComponent& transforms);
	void MoveMotor(MotorComponent& motor, TransformComponent& transforms);
private:
	TimeSystem* m_timeSystem;

	CheckpointList<MotorComponent>* m_motors;
	CheckpointList<TransformComponent>* m_transforms;
};

#endif