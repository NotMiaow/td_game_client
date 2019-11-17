#include "logic.h"

Logic::Logic(std::shared_future<void>&& serverFuture, SharedQueue<Event*>& eventQueue) {
	m_serverFuture = serverFuture;
	m_alive = true;

	m_eventQueue = &eventQueue;

    m_terminateThread = std::thread(&Logic::WaitForTerminate, this);
	m_processThread = std::thread(&Logic::AddEvent, this);
}

Logic::~Logic()
{
    m_terminateThread.join();
	m_processThread.join();
}

void Logic::AddEvent()
{
	while (m_alive)
	{
		if (m_eventQueue->GetSize())
		{
			Event* e = m_eventQueue->Pop();
			std::cout << e->clientId << std::endl;
			if(e != 0) 	m_eventQueue->Push(e);
		}
	}
}

void Logic::WaitForTerminate()
{
    while(m_serverFuture.wait_for(std::chrono::milliseconds(1000)) == std::future_status::timeout) { }
    m_alive = false;
}