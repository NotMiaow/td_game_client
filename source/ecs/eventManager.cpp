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

void EventManager::SpawnUnitGroup()
{
    SpawnUnitGroupEvent* event = dynamic_cast<SpawnUnitGroupEvent*>(m_event);

    //Create unit group's offense
    OffenseComponent offense;
    offense.baseAttackRate = 1;
    offense.baseDamage = 1;
    offense.curAttackRate = offense.baseAttackRate;
    offense.curDamage = offense.baseDamage;

    //Create unit group's transform
    TransformComponent transform;
    transform.position.x = SPAWN_POSITION_X;
    transform.position.y = SPAWN_POSITION_Y;

    //Create unit group's motor
    MotorComponent motor;
    std::queue<Vector2> path;
    motor.path = path;
    motor.behaviour = Move;
    motor.baseSpeed = 2;
    motor.curSpeed = 2;
    motor.normalizedTarget.x = 0;
    motor.normalizedTarget.y = 0;

    //Create unit group
    m_offenses->InsertNode(offense, *m_playerPosition, UNIT_GROUP_OFFENSES);
    m_transforms->InsertNode(transform, *m_playerPosition, UNIT_GROUP_TRANSFORMS);
    m_motors->InsertNode(motor, *m_playerPosition, UNIT_GROUP_MOTORS);

    //Instantiate unit group
    Instantiate("unit_group", Vector2(SPAWN_POSITION_X, SPAWN_POSITION_Y), m_unitGroups);
}

void EventManager::NewPath()
{
    NewPathEvent* event = dynamic_cast<NewPathEvent*>(m_event);

    CheckpointList<MotorComponent>::Iterator motorIt = m_motors->GetIterator(event->playerPosition, UNIT_GROUP_MOTORS);
    CheckpointList<TransformComponent>::Iterator transformIt = m_transforms->GetIterator(event->playerPosition, UNIT_GROUP_TRANSFORMS);
    for(int i = 0; i < event->motorPosition; i++, motorIt++, transformIt++);
    MotorComponent* motor = motorIt.Get();
    TransformComponent* transform = transformIt.Get();

    motor->path = *event->path;
    motor->behaviour = Move;
    float distanceX = motor->path.front().x - transform->position.x;
    float distanceY = motor->path.front().y - transform->position.y;
    float distance = sqrt(pow(distanceX, 2) + pow(distanceY, 2));
    motor->normalizedTarget.x = distanceX / distance;
    motor->normalizedTarget.y = distanceY / distance;
}

void EventManager::BuildTower()
{
    BuildTowerEvent* event = dynamic_cast<BuildTowerEvent*>(m_event);

    //Set remaining gold
    CheckpointList<BankComponent>::Iterator bankIt = m_banks->GetIterator(*m_playerPosition, PLAYER_BANKS);
    bankIt.Get()->gold = event->remainingGold;    

    //Create tower's OffenseComponent
    OffenseComponent offense;
    offense.baseAttackRate = 1;
    offense.baseDamage = 1;
    offense.curAttackRate = offense.baseAttackRate;
    offense.curDamage = offense.baseDamage;

    //Create tower's TransformComponent
    TransformComponent transformComponent;
    transformComponent.position = Vector2(event->position.x, event->position.y);
    
    //Create tower
    m_offenses->InsertNode(offense, *m_playerPosition, TOWER_OFFENSES);
    m_transforms->InsertNode(transformComponent, *m_playerPosition, TOWER_TRANSFORMS);

    //Instantiate tower
    Instantiate("tower", event->position, m_towers);
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
    m_offenses->RemoveNode(towerPosition, *m_playerPosition, TOWER_OFFENSES);

    //Destroy tower
    m_towers->get_child(towerPosition)->queue_free();
}

void EventManager::Instantiate(std::string sceneName, Vector2 position, Node* parent)
{
    //Instantiate unit group
    ResourceLoader* ressourceLoader = ResourceLoader::get_singleton();
    Ref<PackedScene> packedScene = ressourceLoader->load(std::string("res://" + sceneName + ".tscn").c_str());
    RigidBody* scene = (RigidBody*)packedScene->instance();
    if(parent->get_child_count() == 0)
        parent->add_child(scene);
    else
        parent->add_child_below_node(parent->get_child(0), scene);

    //Move tower to position
    const Vector3 realPosition(position.x, scene->get_transform().get_origin().y, position.y);
    Transform transform;
    transform.set_origin(realPosition);
    scene->set_global_transform(transform);
}