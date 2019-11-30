#include "movementSystem.h"

MovementSystem::MovementSystem()
{
}

MovementSystem::MovementSystem(Node* root, CheckpointList<MotorComponent>& motors, CheckpointList<TransformComponent>& transforms)
{
	//Components
	m_motors = &motors;
	m_transforms = &transforms;

	m_unitGroups = root->find_node(String("projectiles"));
    m_projectiles = root->find_node(String("unit_groups"));
}

MovementSystem::~MovementSystem()
{

}

void MovementSystem::Loop(const float& deltaTime)
{
	for(int i = 0; i < MAX_CLIENTS; i++)
	{
		CheckpointList<MotorComponent>::Iterator motorIt = m_motors->GetIterator(i, UNIT_GROUP_MOTORS);
		CheckpointList<TransformComponent>::Iterator transformIt = m_transforms->GetIterator(i, UNIT_GROUP_TRANSFORMS);
		for(int j = 0; !motorIt.End(); j++, motorIt++, transformIt++)
				if (!motorIt.Get()->path.empty()) MoveMotor(deltaTime, j, *motorIt.Get(), *transformIt.Get());
	}
}

void MovementSystem::MoveMotor(const float& deltaTime, const int& index, MotorComponent& motor, TransformComponent& transform)
{
	//Translate motor
    const Vector3 offset(motor.normalizedTarget.x * motor.curSpeed * deltaTime, 0, motor.normalizedTarget.y * motor.curSpeed * deltaTime);
	transform.position.x += offset.x;
	transform.position.y += offset.z;

	//Translate godot object
	((RigidBody*)m_unitGroups->get_child(index))->global_translate(offset);

	//If passed target
	Vector2 targetPosition = motor.path.front();
	if ((motor.normalizedTarget.x > 0 && transform.position.x >= targetPosition.x ||
		motor.normalizedTarget.x < 0 && transform.position.x <= targetPosition.x) || 
		(motor.normalizedTarget.y > 0 && transform.position.y >= targetPosition.y ||
		motor.normalizedTarget.y < 0 && transform.position.y <= targetPosition.y))
	{
		//Set position as target's position
		transform.position.x = targetPosition.x;
		transform.position.y = targetPosition.y;
		
		//Remove target from motor's path
		motor.path.pop();

		//Update normalised target
		if(!motor.path.empty())
		{
			targetPosition = motor.path.front();
			float distanceX = targetPosition.x - transform.position.x;
			float distanceY = targetPosition.y - transform.position.y;
			float distance = sqrt(pow(distanceX, 2) + pow(distanceY, 2));
			motor.normalizedTarget.x = distanceX / distance;
			motor.normalizedTarget.y = distanceY / distance;
		}
		else
		{
			motor.normalizedTarget.x = 0;
			motor.normalizedTarget.y = 0;
		}
	}
}