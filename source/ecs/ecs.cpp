#include "ecs.h"

void ECS::Init(Node* root, Players& players, Banks& banks, Offenses& offenses, Motors& motors, Transforms& transforms)
{
    m_players = &players;
    m_banks = &banks;
    m_offenses = &offenses;
    m_motors = &motors;
    m_transforms = &transforms;

    m_movementSystem = MovementSystem(root, *m_motors, *m_transforms);
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

