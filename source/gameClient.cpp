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
	m_playerPosition = -1;
	m_serverPort = serverPort;
	m_networkManager.Init(m_client, std::move(m_futureObj), m_serverPort, m_eventQueue);

	//Initialise component data structure
	m_players.Initialise({ 1, 1, 1, 1 });
	m_banks.Initialise({ 1, 1, 1, 1 });
	m_motors.Initialise({ 9, 9, 9, 9 });
	m_transforms.Initialise({ 9, 110, 9, 110, 9, 110, 9, 110 });
	m_offenses.Initialise({ 9, 110, 9, 110, 9, 110, 9, 110 });

    m_eventManager.Init(root, m_playerId, m_playerPosition, m_networkManager, m_eventQueue, m_players, m_banks, m_offenses, m_motors, m_transforms);
    m_inputManager.Init(root, m_playerId, m_playerPosition, m_networkManager, m_banks, m_transforms);

	m_ecs.Init(root, m_players, m_banks, m_offenses, m_motors, m_transforms);
}

void GameClient::Update(const float deltaTime, Vector2 mousePos)
{
	m_ecs.Loop(deltaTime);
	m_inputManager.Loop(mousePos);
	m_eventManager.Loop();
}
