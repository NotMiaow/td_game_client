#include "networkManager.h"

using namespace godot;

NetworkManager::NetworkManager(std::shared_future<void> &&clientFuture, const int serverPort, SharedQueue<Event *>& eventQueue)
{
	m_clientFuture = clientFuture;
	m_alive = true;
	m_terminateThread = std::thread(&NetworkManager::WaitForTerminate, this);

	m_connected = false;
	m_serverPort = serverPort;
	m_clientSocket = 0;
	SetUpClientEnvironment();

	m_clientId = PLAYER_ID;
	m_sessionToken = PLAYER_TOKEN;
	m_eventQueue = &eventQueue;
}

NetworkManager::~NetworkManager()
{
}

bool NetworkManager::SetUpClientEnvironment()
{
	if ((m_clientSocket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		Godot::print("socket creation error\n");
		return false;
	}

	m_serverAddress.sin_family = AF_INET;
	m_serverAddress.sin_port = htons(m_serverPort);

	if (inet_pton(AF_INET, "127.0.0.1", &m_serverAddress.sin_addr) <= 0)
	{
		Godot::print("Invalid address / Adress not supported\n");
		return false;
	}

	m_connectionThread = std::thread(&NetworkManager::TryConnect, this);
	return true;
}

void NetworkManager::TryConnect()
{
	while (m_alive)
	{
		if (m_connected)
			std::this_thread::sleep_for(std::chrono::milliseconds(RECONNECTION_ATTEMPT_DELAY));
		else
		{
			if (connect(m_clientSocket, (struct sockaddr *)&m_serverAddress, sizeof(m_serverAddress)) < 0)
			{
				Godot::print(std::string("Connection failed, reattempting in " + std::to_string(RECONNECTION_ATTEMPT_DELAY) + " seconds.").c_str());
				std::this_thread::sleep_for(std::chrono::milliseconds(RECONNECTION_ATTEMPT_DELAY));
			}
			else
			{
				ConnectAction connectAction;
				connectAction.clientId = m_clientId;
				connectAction.sessionToken = m_sessionToken;
				const char* message = connectAction.ToNetworkable().c_str();
				send(m_clientSocket, message, sizeof(message), 0);
				Godot::print(connectAction.ToNetworkable().c_str());
				m_connected = true;
			}
		}
	}
}

void NetworkManager::ListenToServer()
{
	int iResult;
	char buffer[DEFAULT_BUFLEN];
	while (m_alive)
	{
		if (m_connected)
		{
			iResult = read(m_clientSocket, buffer, DEFAULT_BUFLEN);
			if (iResult > 0)
			{
				Event *event = CreateGameEvent(Split(buffer, iResult));
				m_eventQueue->Push(event);
			}
			else
			{
				Godot::print(std::string("Connection lost, attempting reconnection in " + std::to_string(RECONNECTION_ATTEMPT_DELAY) + " seconds.").c_str());
				m_connected = false;
				std::this_thread::sleep_for(std::chrono::milliseconds(RECONNECTION_ATTEMPT_DELAY));
			}
		}
		else
			std::this_thread::sleep_for(std::chrono::milliseconds(RECONNECTION_ATTEMPT_DELAY));
	}
}

void NetworkManager::SendEvent(Event *event)
{
	if (m_connected)
	{
		GameAction gameAction;
		gameAction.clientId = m_clientId;
		gameAction.sessionToken = m_sessionToken;
		gameAction.gameEvent = event->ToNetworkable();
		std::string message = gameAction.ToNetworkable();
		send(m_clientSocket, message.c_str(), message.length(), 0);
	}
}

void NetworkManager::WaitForTerminate()
{
	while (m_clientFuture.wait_for(std::chrono::milliseconds(1000)) == std::future_status::timeout) { }
	m_alive = false;
}