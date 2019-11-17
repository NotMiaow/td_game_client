#ifndef GAME_CLIENT_HPP
#define GAME_CLIENT_HPP

//Godot
#include <Godot.hpp>
#include <Node.hpp>

//Required
#include "shared_queue.h"
#include "cst.h"
#include "action.h"
#include "client.h"
#include "networkManager.h"
#include "ecs.h"

namespace godot
{
class GameClient : public Node
{
	GODOT_CLASS(GameClient, Node)

private:
public:
	//Native
	static void _register_methods();
	void _init();
	void Update(); //Main loop

	//Init of game client
	void InitGameClient(int serverPort);
	void CleanUp();

public:
	//Terminate
	std::atomic<bool> m_alive;
	std::thread m_terminateThread;
	std::promise<void> m_exitSignal;
	std::shared_future<void> m_futureObj;

	//Networking
	int m_serverPort;
	Client m_client;
	NetworkManager* m_networkManager;

	//Game related
	SharedQueue<Event *> m_eventQueue;
	ECS* m_ecs;
};
} // namespace godot

#endif