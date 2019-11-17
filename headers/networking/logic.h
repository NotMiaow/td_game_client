#ifndef LOGIC_H__
#define LOGIC_H__

#include <iostream>
#include <string>
#include <thread>
#include <chrono>
#include <future>

#include "client.h"
#include "cst.h"
#include "event.h"
#include "eventTranslator.h"
#include "shared_queue.h"

class Logic
{
public:
	Logic() { }
	Logic(std::shared_future<void>&& serverFuture, SharedQueue<Event*>& eventQueue);
	~Logic();
	void WaitForTerminate();
	void AddEvent();
private:
	//Terminate
	std::shared_future<void> m_serverFuture;
	std::atomic<bool> m_alive;
	std::thread m_terminateThread;
	
	//Processing Actions
	std::thread m_processThread;
	SharedQueue<Event*>* m_eventQueue;
};

#endif