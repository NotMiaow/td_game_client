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

void GameClient::InitGameClient(int serverPort, Node* root)
{
	//Termination of scene
	m_alive = true;
	m_futureObj = m_exitSignal.get_future();

	//Networking
	m_client.id = PLAYER_ID;
	m_client.token = PLAYER_TOKEN;
	m_serverPort = serverPort;
	m_networkManager.Init(m_client, std::move(m_futureObj), m_serverPort, m_eventQueue);

    m_eventManager.Init(m_networkManager, m_eventQueue, m_players, m_banks, m_motors, m_transforms);
    m_inputManager.Init(root, m_eventQueue);

	m_ecs.Init(m_players, m_banks, m_motors, m_transforms);
}

void GameClient::Update(const float deltaTime, Vector2 mousePos)
{
	m_ecs.Loop(deltaTime);
	m_inputManager.Loop(mousePos);
	m_eventManager.Loop();
}

void GameClient::CleanUp()
{
}
