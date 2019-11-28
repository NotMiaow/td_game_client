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

    for(int i = 0; i < MAX_CLIENTS; i++)
    {
        BankComponent bank;
        bank.gold = 0;
        bank.income = 0;
        m_banks->InsertNode(bank, i, PLAYER_BANKS);
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
        SellTower();
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
    std::vector<PlayerComponent>::iterator player = event->players->begin();
    for(int i = 0; player != event->players->end(); i++, player++)
        m_players->InsertNode(*player, i, PLAYER_PLAYERS);
    //Set bank statuses
    CheckpointList<BankComponent>::Iterator bankIt(m_banks->GetNodeHead(), 0);
    std::vector<BankComponent>::iterator bank = event->banks->begin();
    for(int i = 0; bank != event->banks->end(); i++, bankIt++, bank++)
    {
        BankComponent* bankComponent = bankIt.Get();
        bankComponent->gold = (*bank).gold;
        bankComponent->income = (*bank).income;
    }
}

void EventManager::BuildTower()
{
    BuildTowerEvent* event = dynamic_cast<BuildTowerEvent*>(m_event);

    //Set remaining gold
    CheckpointList<BankComponent>::Iterator bankIt = m_banks->GetIterator(*m_playerPosition, PLAYER_BANKS);
    bankIt.Get()->gold = event->remainingGold;    

    //Store data
    TransformComponent transformComponent;
    transformComponent.position = Vector2(event->position.x, event->position.y);
    m_transforms->InsertNode(transformComponent, *m_playerPosition, TOWER_TRANSFORMS);

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
}

void EventManager::SellTower()
{
    SellTowerEvent* event = dynamic_cast<SellTowerEvent*>(m_event);

    //Set remaining gold
    CheckpointList<BankComponent>::Iterator bankIt = m_banks->GetIterator(*m_playerPosition, PLAYER_BANKS);
    bankIt.Get()->gold = event->remainingGold;    

        //Find the requested tower's position within components
    int towerPosition = 0;
    CheckpointList<TransformComponent>::Iterator transformIt = m_transforms->GetIterator(*m_playerPosition, TOWER_TRANSFORMS);
    for(;!transformIt.End(); towerPosition++, transformIt++)
        if(transformIt.Get()->position.x == event->towerPosition.x && transformIt.Get()->position.y == event->towerPosition.y)
            break;
    
    //If tower does not exist
    if(transformIt.End())
        return;

    //Remove the tower
    m_transforms->RemoveNode(towerPosition, *m_playerPosition, TOWER_TRANSFORMS);
//    m_offenses->RemoveNode(towerPosition, *m_playerPosition, TOWER_OFFENSES);

    //Destroy tower
    m_towers->get_child(towerPosition)->queue_free();
}