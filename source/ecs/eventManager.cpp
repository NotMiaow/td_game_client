#include "eventManager.h"

void EventManager::Init(Node* root, int& playerId, int& playerPosition, NetworkManager& networkManager, SharedQueue<Event*>& eventQueue,
            Players& players, Banks& banks, Offenses& offenses, Motors& motors, Transforms& transforms)
{
    m_playerId = &playerId;
    m_playerPosition = &playerPosition;
    
    m_networkManager = &networkManager;
    m_eventQueue = &eventQueue;

    m_players = &players;
    m_banks = &banks;
    m_offenses = &offenses;
    m_motors = &motors;
    m_transforms = &transforms;

    m_root = root;
    m_towers = root->find_node(String("towers"));
    m_unitGroups = root->find_node(String("unit_groups"));
    m_projectiles = root->find_node(String("projectiles"));
}

EventManager::~EventManager()
{
    
}

void EventManager::Loop()
{
   while (m_eventQueue->size())
    {
        m_event = m_eventQueue->front();
		Godot::print(m_event->ToNetworkable().c_str());
        if(m_event != 0) SwitchEvent();
        delete m_event;
        m_eventQueue->pop_front();
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
        SpawnUnitGroup();
        break;
    case ENewPath:
        NewPath();
        break;
    case ERage:
        SetRage();
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
    
    *m_playerId = event->playerId;
    for(std::tuple<int, std::vector<int>::iterator, std::vector<PlayerComponent>::iterator, std::vector<BankComponent>::iterator> playerIt = std::make_tuple(
            0,
            event->entityIds->begin(),
            event->players->begin(),
            event->banks->begin()
        )
        ; std::get<2>(playerIt) != event->players->end(); std::get<0>(playerIt)++, std::get<1>(playerIt)++, std::get<2>(playerIt)++, std::get<3>(playerIt)++)
    {
        m_players->Add(*std::get<2>(playerIt), *std::get<1>(playerIt), GetCheckpoint(std::get<0>(playerIt), TPlayer, PLAYER_PLAYERS));
        m_banks->Add(*std::get<3>(playerIt), *std::get<1>(playerIt), GetCheckpoint(std::get<0>(playerIt), TBank, PLAYER_BANKS));
    }
    *m_playerPosition = m_players->GetCheckpointPosition(event->playerId);
}

void EventManager::SpawnUnitGroup()
{
    SpawnUnitGroupEvent* event = dynamic_cast<SpawnUnitGroupEvent*>(m_event);


    OffenseComponent offense(1, 1);
    TransformComponent transform(Vector2(SPAWN_POSITION_X, SPAWN_POSITION_Y));
    MotorComponent motor(2);

    const int playerPosition = m_players->GetCheckpointPosition(event->playerId);
    m_offenses->Add(offense, event->entityId, GetCheckpoint(playerPosition, TOffense, UNIT_GROUP_OFFENSES));
    m_transforms->Add(transform, event->entityId, GetCheckpoint(playerPosition, TTransform,  UNIT_GROUP_TRANSFORMS));
    m_motors->Add(motor, event->entityId, GetCheckpoint(playerPosition, TMotor, UNIT_GROUP_MOTORS));

    Transforms::Row unitGroup = m_transforms->GetById(event->entityId);

    Godot::print(std::string("playerId: " + std::to_string(event->playerId) +
                            " | entityId: " + std::to_string(event->entityId) +
                            " | index: " + std::to_string(unitGroup.index)
                            ).c_str());
    Instantiate("unit_group", unitGroup.index, Vector2(SPAWN_POSITION_X, SPAWN_POSITION_Y), m_unitGroups);
}

void EventManager::NewPath()
{
    NewPathEvent* event = dynamic_cast<NewPathEvent*>(m_event);

    Motors::Row motor = m_motors->GetById(event->entityId);
    Transforms::Row transform = m_transforms->GetById(event->entityId);


    Godot::print(std::string("motorId: "+ std::to_string(motor.entityId)).c_str());
    
    motor.data->path = *event->path;
    motor.data->behaviour = Move;
    float distanceX = motor.data->path.front().x - transform.data->position.x;
    float distanceY = motor.data->path.front().y - transform.data->position.y;
    float distance = sqrt(pow(distanceX, 2) + pow(distanceY, 2));
    motor.data->normalizedTarget.x = distanceX / distance;
    motor.data->normalizedTarget.y = distanceY / distance;

    Godot::print("hello");
}

void EventManager::SetRage()
{
    RageEvent* event = dynamic_cast<RageEvent*>(m_event);

    Motors::Row motor = m_motors->GetById(event->entityId);

    motor.data->behaviour = Rage;
    motor.data->normalizedTarget.x = 0;
    motor.data->normalizedTarget.y = 0;
    motor.data->path.clear();
    Godot::print("Rage");
}

void EventManager::BuildTower()
{
    BuildTowerEvent* event = dynamic_cast<BuildTowerEvent*>(m_event);

    Banks::Row bank = m_banks->GetById(event->playerId);
    bank.data->gold = event->remainingGold;    

    OffenseComponent offense(1, 1);
    TransformComponent transform(event->position);
    
    const int playerPosition = m_players->GetCheckpointPosition(event->playerId);
    m_offenses->Add(offense, event->entityId, GetCheckpoint(playerPosition, TOffense, TOWER_OFFENSES));
    m_transforms->Add(transform, event->entityId, GetCheckpoint(playerPosition, TTransform, TOWER_TRANSFORMS));

    Transforms::Row tower = m_transforms->GetById(event->entityId);

    //Instantiate tower
    Instantiate("tower", tower.index, event->position, m_towers);
}

void EventManager::SellTower()
{
    SellTowerEvent* event = dynamic_cast<SellTowerEvent*>(m_event);

    const int playerPosition = m_players->GetCheckpointPosition(event->playerId);
    Banks::Row bank = m_banks->GetById(event->playerId);
    bank.data->gold = event->remainingGold;

    Transforms::Row transform = m_transforms->GetById(event->entityId);
    m_transforms->Remove(event->entityId);
    m_offenses->Remove(event->entityId);
    
    m_towers->get_child(transform.index)->get_child(0)->queue_free();
}

void EventManager::Instantiate(std::string path, const int& index, Vector2 position, Node* parent)
{
    //Instantiate unit group
    ResourceLoader* ressourceLoader = ResourceLoader::get_singleton();
    Ref<PackedScene> packedScene = ressourceLoader->load(std::string("res://" + path + ".tscn").c_str());
    RigidBody* scene = (RigidBody*)packedScene->instance();

    if(parent->get_child(index)->get_child_count() == 0)
        parent->get_child(index)->add_child(scene);

    //Move tower to position
    const Vector3 realPosition(position.x, scene->get_transform().get_origin().y, position.y);
    Transform transform;
    transform.set_origin(realPosition);
    scene->set_global_transform(transform);
}