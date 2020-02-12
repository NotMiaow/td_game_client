#ifndef EVENT_HANDLER_H__
#define EVENT_HANDLER_H__

#include <iostream>
#include <math.h>
#include <string>
#include <vector>
#include <deque>
#include <tuple>

//Godot
#include <Godot.hpp>
#include <ResourceLoader.hpp>
#include <PackedScene.hpp>
#include <RigidBody.hpp>
#include <Vector3.hpp>
#include <Transform.hpp>

//Paths
#include "Vector2.hpp"

#include "componentArray.h"
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

//Misc
#include "definitions.h"

//Network Manager
#include "networkManager.h"

using namespace godot;

class EventManager
{
public:
    EventManager() { }
    ~EventManager();
    void Init(Node* root, int& playerId, int& playerPosition, NetworkManager& networkManager, SharedQueue<Event*>& eventQueue,
            Players& players, Banks& banks, Offenses& offenses, Motors& motors, Transforms& transforms);
    void Loop();
private:
    void SwitchEvent();
    void Connect();
    void Disconnect();
    void ReadyUp();
    void SpawnUnitGroup();
    void NewPath();
    void SetRage();
    void BuildTower();
    void SellTower();
    void Instantiate(std::string path, const int& index, Vector2 position, Node* parent);
private:
    int* m_playerId;
    int* m_playerPosition;
    Event* m_event;
    SharedQueue<Event*>* m_eventQueue;

   	Players* m_players;
    Banks* m_banks;
    Offenses* m_offenses;
   	Motors* m_motors;
	Transforms* m_transforms;

    NetworkManager* m_networkManager;

    Node* m_root;
    Node* m_towers;
    Node* m_unitGroups;
    Node* m_projectiles;
};

#endif
