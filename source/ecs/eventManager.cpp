#include "eventManager.h"

EventManager::EventManager(NetworkManager* networkManager, SharedQueue<Event*>& eventQueue, CheckpointList<PlayerComponent>& players, 
                            CheckpointList<MotorComponent>& motors, CheckpointList<TransformComponent>& transforms)
{
    m_networkManager = networkManager;
    m_eventQueue = &eventQueue;

    m_players = &players;
    m_motors = &motors;
    m_transforms = &transforms;
}

EventManager::~EventManager()
{
    
}

void EventManager::Loop()
{
    while (m_eventQueue->GetSize())
    {
        m_event = m_eventQueue->Pop();
		godot::Godot::print(m_event->ToNetworkable().c_str());
        if(m_event != 0)
        {
            SwitchEvent();
        }
        delete m_event;
    }
}

void EventManager::SwitchEvent()
{
    switch(m_event->GetType())
    {
    case EError:
        break;
    case EConnect:
        Connect();
        break;
    case EDisconnect:
        break;
    case EReadyUp:
        ReadyUp();
        break;
    case ESpawnUnitGroup:
        break;
    case ENewPath:
        break;
    case ERage:
        break;
    case EBuildTower:
        break;
    case ESellTower:
        break;
    case ESendUnitGroup:
        break;
    }
}

void EventManager::Connect()
{
    ConnectEvent* m_event = dynamic_cast<ConnectEvent*>(m_event);
    ReadyUpEvent readyUpEvent;
    m_networkManager->SendEvent(readyUpEvent.ToNetworkable());
}

void EventManager::ReadyUp()
{
    ReadyUpEvent* m_event = dynamic_cast<ReadyUpEvent*>(m_event);
    //Assign resources here
}