#include "eventManager.h"

EventManager::EventManager(NetworkManager* networkManager, SharedQueue<Event*>& eventQueue, CheckpointList<PlayerComponent>& players,
                CheckpointList<BankComponent>& banks,CheckpointList<MotorComponent>& motors, CheckpointList<TransformComponent>& transforms)
{
    m_networkManager = networkManager;
    m_eventQueue = &eventQueue;

    m_players = &players;
    m_banks = &banks;
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
        if(m_event != 0) SwitchEvent();
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
        Disconnect();
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
    ReadyUpEvent readyUpEvent;
    m_networkManager->SendEvent(readyUpEvent.ToNetworkable());
}

void EventManager::Disconnect()
{
    DisconnectEvent* event = dynamic_cast<DisconnectEvent*>(m_event);
    m_networkManager->SendEvent("");
    //End sequence
}

void EventManager::ReadyUp()
{
    ReadyUpEvent* event = dynamic_cast<ReadyUpEvent*>(m_event);
    m_playerPosition = event->playerPosition;

    //Set player statuses
    CheckpointList<PlayerComponent>::Node<PlayerComponent>* pit = m_players->GetNodeHead();
    for(std::vector<PlayerComponent>::iterator event_pit = event->players->begin(); event_pit != event->players->end(); event_pit++)
        pit = m_players->InsertAfterNode(*event_pit, pit);

    //Set bank statuses
    CheckpointList<BankComponent>::Node<BankComponent>* bit = m_banks->GetNodeHead();
    for(std::vector<BankComponent>::iterator event_bit = event->banks->begin(); event_bit != event->banks->end(); event_bit++)
        bit = m_banks->InsertAfterNode(*event_bit, bit);
}