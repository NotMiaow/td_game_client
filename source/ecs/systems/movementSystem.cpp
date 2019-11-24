#include "movementSystem.h"

MovementSystem::MovementSystem()
{
}

MovementSystem::MovementSystem(CheckpointList<MotorComponent>& motors, CheckpointList<TransformComponent>& transforms)
{
	//Components
	m_motors = &motors;
	m_transforms = &transforms;
}

MovementSystem::~MovementSystem()
{

}

void MovementSystem::Loop(const float& deltaTime)
{
	DataNode<MotorComponent>* mNode = m_motors->GetNodeHead();
	DataNode<TransformComponent>* tNode = m_transforms->GetNodeHead();
	while (mNode != NULL)
	{
		SwitchBehaviour(deltaTime, mNode->data, tNode->data);
		mNode = m_motors->GetNextNode(&*mNode);
		tNode = m_transforms->GetNextNode(&*tNode);
	}
}

void MovementSystem::SwitchBehaviour(const float& deltaTime, MotorComponent& motor, TransformComponent& transform)
{
	if (motor.path.GetSize() > 0)
	{
		switch (motor.behaviour)
		{
		case Move:
			MoveMotor(deltaTime, motor, transform);
			break;
		case Rage:
			break;
		case Despawn:
			break;
		default:
			break;
		}
	}
}

void MovementSystem::MoveMotor(const float& deltaTime, MotorComponent& motor, TransformComponent& transform)
{
	//Translate motor
	transform.position.x += motor.normalizedTarget.x * motor.curSpeed * deltaTime;
	transform.position.y += motor.normalizedTarget.y * motor.curSpeed * deltaTime;

	//If passed target
	if (motor.normalizedTarget.x > 0 && transform.position.x >= motor.path.Front().x ||
		motor.normalizedTarget.x < 0 && transform.position.x <= motor.path.Front().x)
	{
		//Set position as target's position
		transform.position.x = motor.path.Front().x;
		transform.position.y = motor.path.Front().y;
		//Remove target from motor's path
		motor.path.Pop();

		//Update normalised target
		if(motor.path.GetSize() > 0)
		{
			float distanceX = motor.path.Front().x - transform.position.x;
			float distanceY = motor.path.Front().y - transform.position.y;
			float distance = sqrt(pow(distanceX, 2) + pow(distanceY, 2));
			motor.normalizedTarget.x = distanceX / distance;
			motor.normalizedTarget.y = distanceY / distance;
		}
	}
}