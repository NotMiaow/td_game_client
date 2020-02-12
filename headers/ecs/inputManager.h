#ifndef INPUT_MANAGER_H__
#define INPUT_MANAGER_H__

#include <math.h>

//Godot
#include <Godot.hpp>
#include <Node.hpp>
#include <String.hpp>
#include <Input.hpp>
#include <Vector2.hpp>
//Instanciation
#include <ResourceLoader.hpp>
#include <PackedScene.hpp>
#include <RigidBody.hpp>
#include <Vector3.hpp>
#include <Transform.hpp>

//Shared
#include "event.h"

#include "componentArray.h"
//Components
#include "transformComponent.h"

//Networking
#include "networkManager.h"

//Misc
#include "definitions.h"
#include "cst.h"
#include "gameEnums.h"

using namespace godot;

class InputManager
{
public:
    InputManager() { };
    ~InputManager();
    void Init(Node* root, int& playerId, int& playerPosition, NetworkManager& networkManager, Banks& banks, Transforms& transforms);
    void Loop(const Vector2& mousePos);
private:
    void MoveTowerPlaceholder(const Vector2& position);
    void DestroyTowerPlaceholder();
    bool BuildTower(const Vector2& position);
    void SellTower(const Vector2& position);
    void SpawnUnit();
    bool TowerExists(const Vector2& position);
private:
    int* m_playerId;
    int* m_playerPosition;
    NetworkManager* m_networkManager;
    
    Banks* m_banks;    
    Transforms* m_transforms;

    Node* m_root;
    Node* m_towerPlaceholderParent;
    RigidBody* m_towerPlaceholder;
    bool m_buildMode;
};

#endif