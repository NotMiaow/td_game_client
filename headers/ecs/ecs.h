#ifndef ECS_H__
#define ECS_H__

#include <iostream>
#include <thread>
#include <chrono>
#include <future>
#include <atomic>


//Godot
#include <Godot.hpp>
#include <Vector2.hpp>
#include <Node.hpp>

#include "componentArray.h"
//Components
#include "playerComponent.h"
#include "bankComponent.h"
#include "motorComponent.h"
#include "transformComponent.h"

//Systems
#include "movementSystem.h"

//Events
#include "shared_queue.h"
#include "event.h"

//Misc
#include "definitions.h"

using namespace godot;

class ECS
{
public:
	ECS() {}
	~ECS();
	void Init(Node* root, Players& players, Banks& banks, Offenses& offenses, Motors& motors, Transforms& transforms);
	bool Loop(const float deltaTime);
private:
	void WaitForTerminate();
private:
	//Components
	Players* m_players;
	Banks* m_banks;
	Offenses* m_offenses;
	Motors* m_motors;
	Transforms* m_transforms;
	
	//Systems
	MovementSystem m_movementSystem;

	//Event queue
	SharedQueue<Event*>* m_eventQueue;
};


#endif
