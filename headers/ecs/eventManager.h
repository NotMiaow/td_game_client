#ifndef EVENT_HANDLER_H__
#define EVENT_HANDLER_H__

#include <iostream>
#include <math.h>

//Godot
#include "Godot.hpp"

//Paths
#include "Vector2.hpp"
#include "queue.h"

#include "checkpointList.h"
//Components
#include "client.h"
#include "playerComponent.h"
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
    EventManager(NetworkManager* networkManager, SharedQueue<Event*>& eventQueue, CheckpointList<PlayerComponent>& players, 
                    CheckpointList<MotorComponent>& motors, CheckpointList<TransformComponent>& transforms);
    ~EventManager();
    void Loop();
private:
    void SwitchEvent();
    void ConnectPlayer();
private:
    Event* event;
    SharedQueue<Event*>* m_eventQueue;

   	CheckpointList<PlayerComponent>* m_players;
   	CheckpointList<MotorComponent>* m_motors;
	CheckpointList<TransformComponent>* m_transforms;

    NetworkManager* m_networkManager;
};

#endif
