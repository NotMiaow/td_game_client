#include "ecs.h"

void ECS::Init(CheckpointList<PlayerComponent>& players, CheckpointList<BankComponent>& banks,CheckpointList<MotorComponent>& motors,
        CheckpointList<TransformComponent>& transforms)
{
    m_players = &players;
    m_banks = &banks;
    m_motors = &motors;
    m_transforms = &transforms;

    m_timeSystem = TimeSystem();
    m_movementSystem = MovementSystem(*m_motors, *m_transforms);
}

ECS::~ECS()
{
}

bool ECS::Loop()
{
    m_timeSystem.Loop();
    m_movementSystem.Loop();
    
    //Kill server from within ecs
//    return false;
    return true;
}

