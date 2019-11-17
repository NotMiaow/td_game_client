#include "eventManager.h"

EventManager::EventManager(NetworkManager* networkManager, SharedQueue<Event*>& eventQueue, CheckpointList<PlayerComponent>& players, 
                            CheckpointList<MotorComponent>& motors, CheckpointList<TransformComponent>& transforms)
{
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
        event = m_eventQueue->Pop();
        if(event != 0)
        {
			godot::Godot::print(event->ToNetworkable().c_str());
            SwitchEvent();
        }
        delete event;
    }
}

void EventManager::SwitchEvent()
{
    switch(event->GetType())
    {
    case EError:
        break;
    case EConnect:
        ConnectPlayer();
        godot::Godot::print("niceuh1");
        break;
    case EDisconnect:
        break;
    case EReadyUp:
        godot::Godot::print("niceuh2");
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

void EventManager::ConnectPlayer()
{
    ConnectEvent* event = dynamic_cast<ConnectEvent*>(event);
    ReadyUpEvent readyUpEvent;
    m_networkManager->SendEvent(readyUpEvent.ToNetworkable());
}