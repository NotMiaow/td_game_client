#include "gameClient.h"

using namespace godot;

void GameClient::_register_methods()
{
	register_method("Update", &GameClient::Update);
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

	m_client.id = PLAYER_ID;
	m_client.token = PLAYER_TOKEN;
	m_eventQueue = SharedQueue<Event *>();
	m_networkManager = new NetworkManager(m_client, std::move(m_futureObj), m_serverPort, m_eventQueue);
	m_ecs = new ECS(m_networkManager, m_eventQueue);
}

void GameClient::Update()
{
	m_ecs->Loop();
}

void GameClient::CleanUp()
{
}
