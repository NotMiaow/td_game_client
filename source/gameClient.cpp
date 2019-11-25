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

	InitialiseComponentDataStructure();

    m_eventManager.Init(root, m_playerPosition, m_networkManager, m_eventQueue, m_players, m_banks, m_offenses, m_motors, m_transforms);
    m_inputManager.Init(root, m_playerPosition, m_networkManager, m_banks, m_transforms);

	m_ecs.Init(m_players, m_banks, m_offenses, m_motors, m_transforms);
}

void GameClient::Update(const float deltaTime, Vector2 mousePos)
{
	m_ecs.Loop(deltaTime);
	m_inputManager.Loop(mousePos);
	m_eventManager.Loop();
}

void GameClient::InitialiseComponentDataStructure()
{
	//PlayerComponent
	{
		TabNode<PlayerComponent>* tabIt = m_players.GetTabHead();
		for(int i = 0; i < MAX_CLIENTS; i++)
			tabIt = m_players.InsertTab(tabIt);
	}
	//BankComponent
	{
		TabNode<BankComponent>* tabIt = m_banks.GetTabHead();
		for(int i = 0; i < MAX_CLIENTS; i++)
			tabIt = m_banks.InsertTab(tabIt);
	}
	//OffenseComponent
	{
		TabNode<OffenseComponent>* tabIt = m_offenses.GetTabHead();
		CheckpointNode<OffenseComponent>* checkpointIt;
		for(int i = 0; i < 1; i++)
		{
			tabIt = m_offenses.InsertTab(tabIt);
			checkpointIt = tabIt->checkpointNode;
			for(int i = 1; i < 2; i++)
				checkpointIt = m_offenses.InsertCheckpoint(checkpointIt);
		}
	}
	//Motors
	{
		TabNode<MotorComponent>* tabIt = m_motors.GetTabHead();
		for(int i = 0; i< 1; i++)
			tabIt = m_motors.InsertTab(tabIt);
	}
	//TransformComponent
	{
		TabNode<TransformComponent>* tabIt = m_transforms.GetTabHead();
		CheckpointNode<TransformComponent>* checkpointIt;
		for(int i = 0; i < 1; i++)
		{
			tabIt = m_transforms.InsertTab(tabIt);
			checkpointIt = tabIt->checkpointNode;
			for(int i = 1; i < 2; i++)
				checkpointIt = m_transforms.InsertCheckpoint(checkpointIt);
		}
	}
}
