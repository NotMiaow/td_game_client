#ifndef GAME_CLIENT_HPP
#define GAME_CLIENT_HPP

#include <string>

//Godot
#include <Godot.hpp>
#include <Node.hpp>
#include <Camera.hpp>
#include <PhysicsDirectSpaceState.hpp>

#include "componentArray.h"
//Components
#include "playerComponent.h"
#include "bankComponent.h"
#include "offenseComponent.h"
#include "motorComponent.h"
#include "transformComponent.h"

//Shared
#include "shared_queue.h"
#include "event.h"

//Networking
#include "action.h"
#include "client.h"

//Managers
#include "networkManager.h"
#include "eventManager.h"
#include "inputManager.h"

//Misc
#include "cst.h"
#include "ecs.h"

namespace godot
{
class GameClient : public Camera
{
	GODOT_CLASS(GameClient, Node)

private:
public:
	//Native
	static void _register_methods();
	void _init();
	void Update(const float deltaTime, Vector2 mousePos); //Main loop

	//Init of game client
	void InitGameClient(int serverPort, Node* root);

public:
	//Terminate
	std::atomic<bool> m_alive;
	std::thread m_terminateThread;
	std::promise<void> m_exitSignal;
	std::shared_future<void> m_futureObj;

	//Shared resources
	SharedQueue<Event *> m_eventQueue;

	//Components	
	Players m_players;
	Banks m_banks;
	Offenses m_offenses;
	Motors m_motors;
	Transforms m_transforms;

	//Networking
	int m_serverPort;
	int m_playerId;
	int m_playerPosition;
	Client m_client;
	NetworkManager m_networkManager;

	EventManager m_eventManager;
	InputManager m_inputManager;

	ECS m_ecs;
};
} // namespace godot

#endif