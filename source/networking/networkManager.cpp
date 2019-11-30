#include "networkManager.h"

void NetworkManager::Init(Client &client, std::shared_future<void> &&clientFuture, const int serverPort, SharedQueue<Event *> &eventQueue)
{
	m_clientFuture = clientFuture;
	m_alive = true;
	m_terminateThread = std::thread(&NetworkManager::WaitForTerminate, this);

	m_connected = false;
	m_serverPort = serverPort;
	m_clientSocket = 0;
	SetUpClientEnvironment();

	m_client = client;
	m_eventQueue = &eventQueue;
}

NetworkManager::~NetworkManager()
{
	m_connectionThread.join();
	m_listeningThread.join();
}

bool NetworkManager::SetUpClientEnvironment()
{
	if ((m_clientSocket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		godot::Godot::print("socket creation error\n");
		return false;
	}

	m_serverAddress.sin_family = AF_INET;
	m_serverAddress.sin_port = htons(m_serverPort);

	if (inet_pton(AF_INET, "127.0.0.1", &m_serverAddress.sin_addr) <= 0)
	{
		godot::Godot::print("Invalid address / Adress not supported\n");
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
				godot::Godot::print(std::string("Connection failed, reattempting in " + std::to_string(RECONNECTION_ATTEMPT_DELAY) + " seconds.").c_str());
				std::this_thread::sleep_for(std::chrono::milliseconds(RECONNECTION_ATTEMPT_DELAY));
			}
			else
			{
				m_connected = true;
				m_listeningThread = std::thread(&NetworkManager::ListenToServer, this);

				ConnectAction connectAction;
				connectAction.clientId = m_client.id;
				connectAction.sessionToken = m_client.token;
				std::string message = connectAction.ToNetworkable().c_str();
				message = std::to_string(message.length()) + message;
				send(m_clientSocket, message.c_str(), message.length(), 0);
			}
		}
	}
}

void NetworkManager::ListenToServer()
{
	int iResult;
	char recvbuf[BUFFER_LENGTH];
	int recvbuflen = BUFFER_LENGTH;
	std::string message = "";
	int messageLength = 0;
	while (m_alive && m_connected)
	{
		iResult = read(m_clientSocket, recvbuf, recvbuflen);
		if (iResult > 0)
		{
			//Convert to string
			for(int i = 0; i < iResult; i++)
				message += recvbuf[i];
			
			//Get message length if not already
			if(messageLength == 0)
			{
				messageLength = GetMessageLength(message);
				if(messageLength == 0)
				{
					m_connected = false;
					continue;
				}
			}
			//Done receiving
			if(message.length() == messageLength)
			{
				Event *event = CreateGameEvent(Split(message, message.length()));
	 			m_eventQueue->push_back(event);
				message = "";
				messageLength = 0;
			}
			//Error
			else if(message.length() > messageLength)
			{
				m_connected = false;
				message = "";
				messageLength = 0;
			}
 		}
		else
		{
			godot::Godot::print(std::string("Connection lost, attempting reconnection in " + std::to_string(RECONNECTION_ATTEMPT_DELAY) + " seconds.").c_str());
			m_connected = false;
			std::this_thread::sleep_for(std::chrono::milliseconds(RECONNECTION_ATTEMPT_DELAY));
		}
	}
}

int NetworkManager::GetMessageLength(std::string& cutMessage)
{
	int iMessageLength = 0;
	std::string sMessageLength = "";
	int pos = cutMessage.find('{');
	if(pos != std::string::npos)
	{
		sMessageLength = cutMessage.substr(0,pos);
		iMessageLength = std::stoi(sMessageLength);
		cutMessage = cutMessage.substr(pos);
	}
	return iMessageLength;
}

void NetworkManager::SendEvent(const std::string &event)
{
	if (m_connected)
	{
		GameAction gameAction;
		gameAction.clientId = m_client.id;
		gameAction.sessionToken = m_client.token;
		gameAction.gameEvent = event;
		std::string message = gameAction.ToNetworkable();
		message = std::to_string(message.length()) + message;


		std::string cutMessage;
		int i;
		for(i = 0; i + BUFFER_LENGTH < message.length(); i += BUFFER_LENGTH)
		{
			cutMessage = message.substr(i, BUFFER_LENGTH);
			if(!SendString(cutMessage))
				return;
		}	
		if(i < message.length())
		{
			cutMessage = message.substr(i);
			if(!SendString(cutMessage))
				return;
		}	
	}
}

bool NetworkManager::SendString(const std::string& cutMessage)
{
	int iResult = send(m_clientSocket, cutMessage.c_str(), (int)cutMessage.length(), 0);
	if(iResult <= 0)
	{
		m_connected = false;
		return false;
	}
}

void NetworkManager::WaitForTerminate()
{
	while (m_clientFuture.wait_for(std::chrono::milliseconds(1000)) == std::future_status::timeout);
	m_alive = false;
}