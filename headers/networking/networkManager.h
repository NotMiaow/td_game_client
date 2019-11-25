#ifndef NETWORK_MANAGER_H__
#define NETWORK_MANAGER_H__

//Godot
#include <Godot.hpp>

//Networking
#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <string.h> 

//Threading
#include <thread>
#include <chrono>
#include <future>
#include <string>

//Required
#include "shared_queue.h"
#include "action.h"
#include "actionLanguage.h"
#include "event.h"
#include "eventTranslator.h"
#include "cst.h"
#include "client.h"

using namespace godot;

class NetworkManager
{
public:
	NetworkManager() { }
	~NetworkManager();
	void Init(Client& client, std::shared_future<void> &&clientFuture, const int serverPort, SharedQueue<Event *>& eventQueue);
	void SendEvent(const std::string& event);
private:
	bool SetUpClientEnvironment();
	void TryConnect();
	void ListenToServer();
	int GetMessageLength(std::string& cutMessage);
	bool SendString(const std::string& cutMessage);
	void WaitForTerminate();
private:
	//Terminating
	std::shared_future<void> m_clientFuture;
	std::atomic<bool> m_alive;
	std::thread m_terminateThread;

	//Networking
	std::atomic<bool> m_connected;
	int m_serverPort;
	int m_clientSocket;
	struct sockaddr_in m_serverAddress;
	std::thread m_connectionThread;
	std::thread m_listeningThread;

	//Communicating
	Client m_client;
	SharedQueue<Event*>* m_eventQueue;
};

#endif
