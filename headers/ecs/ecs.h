#ifndef ECS_H__
#define ECS_H__

#include <iostream>
#include <thread>
#include <chrono>
#include <future>
#include <atomic>

//Libraries
#include "shared_queue.h"

//Godot
#include <Godot.hpp>
#include <Vector2.hpp>
#include <Node.hpp>

//Components
#include "playerComponent.h"
#include "bankComponent.h"
#include "motorComponent.h"
#include "transformComponent.h"
#include "checkpointList.h"

//Systems
#include "movementSystem.h"

//Misc
#include "eventManager.h"

using namespace godot;

class ECS
{
public:
	ECS() {}
	~ECS();
	void Init(Node* root, CheckpointList<PlayerComponent>& players, CheckpointList<BankComponent>& banks,
	CheckpointList<OffenseComponent>& offenses, CheckpointList<MotorComponent>& motors,	CheckpointList<TransformComponent>& transforms);
	bool Loop(const float deltaTime);
private:
	void WaitForTerminate();
private:
	//Components
	CheckpointList<PlayerComponent>* m_players;
	CheckpointList<BankComponent>* m_banks;
	CheckpointList<OffenseComponent>* m_offenses;
	CheckpointList<MotorComponent>* m_motors;
	CheckpointList<TransformComponent>* m_transforms;
	
	//Systems
	MovementSystem m_movementSystem;

	//Event queue
	SharedQueue<Event*>* m_eventQueue;
};


#endif
