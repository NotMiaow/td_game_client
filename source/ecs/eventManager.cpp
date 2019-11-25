#include "eventManager.h"

void EventManager::Init(Node* root, int& playerPosition, NetworkManager& networkManager, SharedQueue<Event*>& eventQueue, CheckpointList<PlayerComponent>& players,
                CheckpointList<BankComponent>& banks, CheckpointList<OffenseComponent>& offenses, CheckpointList<MotorComponent>& motors,
                CheckpointList<TransformComponent>& transforms)
{
    m_playerPosition = &playerPosition;
    
    m_networkManager = &networkManager;
    m_eventQueue = &eventQueue;

    m_players = &players;
    m_banks = &banks;
    m_offenses = &offenses;
    m_motors = &motors;
    m_transforms = &transforms;

    {
        TabNode<BankComponent>* tabIt;
        for(tabIt = m_banks->GetTabHead(); tabIt; tabIt = m_banks->GetNextTab(&*tabIt))
        {
            BankComponent bank;
            bank.gold = 0;
            bank.income = 0;
            m_banks->InsertNode(bank, tabIt->checkpointNode);
        }
    }

    m_root = root;
    m_towers = root->find_node(String("towers"));
    m_unitGroups = root->find_node(String("projectiles"));
    m_projectiles = root->find_node(String("unit_groups"));
}

EventManager::~EventManager()
{
    
}

void EventManager::Loop()
{
   while (m_eventQueue->GetSize())
    {
        m_event = m_eventQueue->Pop();
		Godot::print(m_event->ToNetworkable().c_str());
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
        BuildTower();
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
    *m_playerPosition = event->playerPosition;

    //Set player statuses
    {
        TabNode<PlayerComponent>* tabIt = m_players->GetTabHead();
        for(std::vector<PlayerComponent>::iterator player = event->players->begin(); player != event->players->end(); player++, tabIt = m_players->GetNextTab(&*tabIt))
            m_players->InsertNode(*player, tabIt->checkpointNode);
    }
    //Set bank statuses
    {
        TabNode<BankComponent>* tabIt = m_banks->GetTabHead();
        for(std::vector<BankComponent>::iterator bank = event->banks->begin(); bank != event->banks->end(); bank++, tabIt = m_banks->GetNextTab(&*tabIt))
            tabIt->checkpointNode->node->data = *bank;
    }
}

void EventManager::BuildTower()
{
    BuildTowerEvent* event = dynamic_cast<BuildTowerEvent*>(m_event);

    //Set remaining gold
    DataNode<BankComponent>* bankIt = m_banks->GetNodeHead();
    for(int i = 0; i < *m_playerPosition; i++, bankIt = m_banks->GetNextNode(&*bankIt));
    bankIt->data.gold = event->remainingGold;    

    //Instantiate tower
    ResourceLoader* ressourceLoader = ResourceLoader::get_singleton();
    Ref<PackedScene> packedScene = ressourceLoader->load("res://tower.tscn");
    RigidBody* tower = (RigidBody*)packedScene->instance();
    if(m_towers->get_child_count() == 0)
        m_towers->add_child(tower);
    else
        m_towers->add_child_below_node(m_towers->get_child(0), tower);
    
    //Move tower to position
    const Vector3 towerPosition(event->position.x + 0.5f, tower->get_transform().get_origin().y, event->position.y + 0.5f);
    Transform transform;
    transform.set_origin(towerPosition);
    tower->set_global_transform(transform);
    
    //Store data
    CheckpointNode<TransformComponent>* checkIt = m_transforms->GetTabHead()->checkpointNode;
    for(int i = 0; i < T_TOWER; i++, checkIt = m_transforms->GetNextCheckpoint(&*checkIt));

    TransformComponent transformComponent;
    transformComponent.position = Vector2(event->position.x, event->position.y);
    m_transforms->InsertNode(transformComponent, checkIt);
}