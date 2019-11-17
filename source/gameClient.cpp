#include "gameClient.h"

using namespace godot;

void GameClient::_register_methods()
{
	register_method("Iteration", &GameClient::Update);
	register_method("InitGameClient", &GameClient::InitGameClient);
}

void GameClient::_init()
{
}

void GameClient::InitGameClient(int serverPort)
{
	m_serverPort = serverPort;

	m_alive = true;
	m_futureObj = m_exitSignal.get_future();

	m_eventQueue = SharedQueue<Event*>();
	m_networkManager = new NetworkManager(std::move(m_futureObj), m_serverPort, m_eventQueue);
//	m_logic = new Logic(std::move(m_futureObj), m_eventQueue);
}

void GameClient::Update()
{
	
}