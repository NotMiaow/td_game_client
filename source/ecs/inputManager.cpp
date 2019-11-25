#include "inputManager.h"

InputManager::~InputManager()
{

}

void InputManager::Init(Node* root, int& playerPosition, NetworkManager& networkManager, CheckpointList<BankComponent>& banks,
    CheckpointList<TransformComponent>& transforms)
{
    m_playerPosition = &playerPosition;
    m_networkManager = &networkManager;

    m_banks = &banks;
    m_transforms = &transforms;

    m_root = root;
    m_towerPlaceholderParent = m_root->find_node(String("tower_placeholder"));
    m_buildMode = false;
}

void InputManager::Loop(const Vector2& mousePos)
{
    int posX = floor(mousePos.x);
    int posY = floor(mousePos.y);
    const Vector2 floorTarget(posX, posY);
    if(Input::get_singleton()->is_action_just_released(String("build_mode")))
    {
        m_buildMode = !m_buildMode;
        if(!m_buildMode) DestroyTowerPlaceholder();
    }
    if(m_buildMode)
    {
        if(mousePos.x < GRID_SIZE_X && mousePos.x >= 0 && mousePos.y < GRID_SIZE_Y && mousePos.y >= 0)
        {
            if(!TowerExists(floorTarget))
            {
                MoveTowerPlaceholder(floorTarget);
                if(Input::get_singleton()->is_action_just_released(String("mouse_1")) && BuildTower(floorTarget))
                    DestroyTowerPlaceholder();
            }
            else DestroyTowerPlaceholder();                
        }
    }
}

void InputManager::MoveTowerPlaceholder(const Vector2& position)
{
    if(m_towerPlaceholderParent->get_child_count() == 0)
    {
        ResourceLoader* ressourceLoader = ResourceLoader::get_singleton();
        Ref<PackedScene> packedScene = ressourceLoader->load("res://towerPlaceholder.tscn");
        m_towerPlaceholder = (RigidBody*)packedScene->instance();
        m_towerPlaceholderParent->add_child(m_towerPlaceholder);
    }
    const Vector3 towerPosition(position.x + 0.5f, m_towerPlaceholder->get_transform().get_origin().y, position.y + 0.5f);
    Transform transform;
    transform.set_origin(towerPosition);
    m_towerPlaceholder->set_global_transform(transform);
}

void InputManager::DestroyTowerPlaceholder()
{
    if(m_towerPlaceholderParent->get_child_count() > 0)
        m_towerPlaceholder->queue_free();
}

bool InputManager::BuildTower(const Vector2& position)
{
    DataNode<BankComponent>* bankIt = m_banks->GetNodeHead();
    for(int i = 0; i < *m_playerPosition; i++, bankIt = m_banks->GetNextNode(&*bankIt));
    if(bankIt->data.gold >= TOWER_COSTS[0])
    {
        BuildTowerEvent buildTower;
        buildTower.position = position;
        buildTower.towerType = 0;
        m_networkManager->SendEvent(buildTower.ToNetworkable());
        return true;
    }
    return false;
}

bool InputManager::TowerExists(const Vector2& position)
{
    CheckpointNode<TransformComponent>* checkIt = m_transforms->GetTabHead()->checkpointNode;
    for(int i = 0; i < T_TOWER; i++)
        checkIt = m_transforms->GetNextCheckpoint(&*checkIt);

    DataNode<TransformComponent>* nodeIt = checkIt->node;
    while (nodeIt && nodeIt != checkIt->next->node)
    {
        if(nodeIt->data.position.x == position.x && nodeIt->data.position.y == position.y)
            return true;
        nodeIt = m_transforms->GetNextNode(&*nodeIt);
    }
    return false;
}

