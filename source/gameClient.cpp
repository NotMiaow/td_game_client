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
	m_players.Init(MAX_CLIENTS, PLAYER_CHECKPOINT_SIZE);
	m_banks.Init(MAX_CLIENTS, BANK_CHECKPOINT_SIZE);
	m_motors.Init(MAX_CLIENTS, MOTOR_CHECKPOINT_SIZE);
	m_transforms.Init(MAX_CLIENTS, TRANSFORM_CHECKPOINT_SIZE);
	m_offenses.Init(MAX_CLIENTS, OFFENSE_CHECKPOINT_SIZE);

    m_eventManager.Init(root, m_playerPosition, m_networkManager, m_eventQueue, m_players, m_banks, m_offenses, m_motors, m_transforms);
    m_inputManager.Init(root, m_playerPosition, m_networkManager, m_banks, m_transforms);

	m_ecs.Init(root, m_players, m_banks, m_offenses, m_motors, m_transforms);
}

void GameClient::Update(const float deltaTime, Vector2 mousePos)
{
	m_ecs.Loop(deltaTime);
	m_inputManager.Loop(mousePos);
	m_eventManager.Loop();
}
