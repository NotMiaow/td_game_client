#ifndef ECS_H__
#define ECS_H__

#include <iostream>
#include <thread>
#include <chrono>
#include <future>
#include <atomic>

//Godot
#include "Vector2.hpp"

//Components
#include "playerComponent.h"
#include "motorComponent.h"
#include "transformComponent.h"
#include "checkpointList.h"

//Systems
#include "timeSystem.h"
#include "movementSystem.h"

//Event Manager
#include "queue.h"
#include "event.h"
#include "eventManager.h"

//Network Manager
#include "networkManager.h"

class ECS
{
public:
	ECS() {}
	ECS(NetworkManager* networkmanager, SharedQueue<Event*>& eventQueue);
	~ECS();
	bool Loop();
private:
	void WaitForTerminate();
private:
	//Components
	CheckpointList<PlayerComponent> m_players;
	CheckpointList<MotorComponent> m_motors;
	CheckpointList<TransformComponent> m_transforms;
	
	//Systems
	TimeSystem m_timeSystem;
	MovementSystem m_movementSystem;
	
	//Event mang
	EventManager m_eventManager;

	//Networking
	NetworkManager* m_networkManager;
};


#endif
