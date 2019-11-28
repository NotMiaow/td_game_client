#ifndef EVENT_HANDLER_H__
#define EVENT_HANDLER_H__

#include <iostream>
#include <math.h>
#include <string>
#include <vector>

//Godot
#include <Godot.hpp>
#include <ResourceLoader.hpp>
#include <PackedScene.hpp>
#include <RigidBody.hpp>
#include <Vector3.hpp>
#include <Transform.hpp>

//Paths
#include "Vector2.hpp"
#include "queue.h"

#include "dataNode.h"
#include "checkpointNode.h"
#include "tabNode.h"
#include "checkpointList.h"
//Components
#include "client.h"
#include "playerComponent.h"
#include "bankComponent.h"
#include "offenseComponent.h"
#include "motorComponent.h"
#include "transformComponent.h"

//Events
#include "eventLanguage.h"
#include "event.h"
#include "shared_queue.h"

//Network Manager
#include "networkManager.h"

using namespace godot;

class EventManager
{
public:
    EventManager() { }
    ~EventManager();
    void Init(Node* root, int& playerPosition, NetworkManager& networkManager, SharedQueue<Event*>& eventQueue, CheckpointList<PlayerComponent>& players,
                CheckpointList<BankComponent>& banks, CheckpointList<OffenseComponent>& offenses, CheckpointList<MotorComponent>& motors,
                CheckpointList<TransformComponent>& transforms);
    void Loop();
private:
    void SwitchEvent();
    void Connect();
    void Disconnect();
    void ReadyUp();
    void BuildTower();
    void SellTower();
private:
    int* m_playerPosition;
    Event* m_event;
    SharedQueue<Event*>* m_eventQueue;

   	CheckpointList<PlayerComponent>* m_players;
    CheckpointList<BankComponent>* m_banks;
    CheckpointList<OffenseComponent>* m_offenses;
   	CheckpointList<MotorComponent>* m_motors;
	CheckpointList<TransformComponent>* m_transforms;

    NetworkManager* m_networkManager;

    Node* m_root;
    Node* m_towers;
    Node* m_unitGroups;
    Node* m_projectiles;
};

#endif
