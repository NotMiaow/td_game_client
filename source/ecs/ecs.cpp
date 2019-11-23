#include "ecs.h"

void ECS::Init(CheckpointList<PlayerComponent>& players, CheckpointList<BankComponent>& banks,CheckpointList<MotorComponent>& motors,
        CheckpointList<TransformComponent>& transforms)
{
    m_players = &players;
    m_banks = &banks;
    m_motors = &motors;
    m_transforms = &transforms;

    m_movementSystem = MovementSystem(*m_motors, *m_transforms);
}

ECS::~ECS()
{
}

bool ECS::Loop(const float deltaTime)
{
    m_movementSystem.Loop(deltaTime);
    
    //Kill server from within ecs
//    return false;
    return true;
}

