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
    CheckpointList<BankComponent>::Iterator bankIt = m_banks->GetIterator(*m_playerPosition, PLAYER_BANKS);
    
    if(bankIt.Get()->gold >= TOWER_COSTS[0])
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
    CheckpointList<TransformComponent>::Iterator transformIt = m_transforms->GetIterator(*m_playerPosition, TOWER_TRANSFORMS);
    for(;!transformIt.End(); transformIt++)
        if(transformIt.Get()->position.x == position.x && transformIt.Get()->position.y == position.y)
           return true;
    return false;
}

