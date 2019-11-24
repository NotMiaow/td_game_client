#include "inputManager.h"

InputManager::~InputManager()
{

}

void InputManager::Init(Node* root, NetworkManager& networkManager, CheckpointList<TransformComponent>& transforms)
{
    m_networkManager = &networkManager;

    m_transforms = &transforms;

    m_root = root;
}

void InputManager::Loop(const Vector2& mousePos)
{
    if(Input::get_singleton()->is_action_just_released(String("mouse_1")) && !(mousePos.x == 0 && mousePos.y == 0))
    {
        if(mousePos.x < GRID_SIZE_X && mousePos.x > 0 && mousePos.y < GRID_SIZE_Y && mousePos.y > 0)
        {
            int posX = floor(mousePos.x);
            int posY = floor(mousePos.y);

            const Vector2 floorTarget(posX, posY);
            BuildTower(floorTarget);
        } 
    }
}

void InputManager::BuildTower(const Vector2& position)
{
    CheckpointNode<TransformComponent>* checkIt = m_transforms->GetTabHead()->checkpointNode;
    for(int i = 0; i < T_TOWER; i++)
        checkIt = m_transforms->GetNextCheckpoint(&*checkIt);

    //FIX existing tower not being seen

    DataNode<TransformComponent>* nodeIt = checkIt->node;
    while (nodeIt && nodeIt != checkIt->next->node)
    {
        if(nodeIt->data.position.x == position.x && nodeIt->data.position.y == position.y)
            return;
        nodeIt = m_transforms->GetNextNode(&*nodeIt);
    }

    BuildTowerEvent buildTower;
    buildTower.position = position;
    buildTower.towerType = 0;
    m_networkManager->SendEvent(buildTower.ToNetworkable());
}

