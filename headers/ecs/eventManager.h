#ifndef EVENT_HANDLER_H__
#define EVENT_HANDLER_H__

#include <iostream>
#include <math.h>
#include <string>
#include <vector>

//Godot
#include "Godot.hpp"

//Paths
#include "Vector2.hpp"
#include "queue.h"

#include "checkpointList.h"
//Components
#include "client.h"
#include "playerComponent.h"
#include "bankComponent.h"
#include "motorComponent.h"
#include "transformComponent.h"

//Events
#include "eventLanguage.h"
#include "event.h"
#include "shared_queue.h"

//Network Manager
#include "networkManager.h"

class EventManager
{
public:
    EventManager() { }
    ~EventManager();
    void Init(NetworkManager& networkManager, SharedQueue<Event*>& eventQueue, CheckpointList<PlayerComponent>& players,
                CheckpointList<BankComponent>& banks,CheckpointList<MotorComponent>& motors, CheckpointList<TransformComponent>& transforms);
    void Loop();
private:
    void SwitchEvent();
    void Connect();
    void Disconnect();
    void ReadyUp();
private:
    int m_playerPosition;
    Event* m_event;
    SharedQueue<Event*>* m_eventQueue;

   	CheckpointList<PlayerComponent>* m_players;
    CheckpointList<BankComponent>* m_banks;
   	CheckpointList<MotorComponent>* m_motors;
	CheckpointList<TransformComponent>* m_transforms;

    NetworkManager* m_networkManager;
};

#endif
