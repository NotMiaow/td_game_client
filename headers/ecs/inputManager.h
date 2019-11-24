#ifndef INPUT_MANAGER_H__
#define INPUT_MANAGER_H__

#include <math.h>

//Godot
#include <Godot.hpp>
#include <Node.hpp>
#include <String.hpp>
#include <Input.hpp>
#include <Vector2.hpp>

//Shared
#include "event.h"

#include "checkpointList.h"
//Components
#include "transformComponent.h"

//Networking
#include "networkManager.h"

//Misc
#include "cst.h"

using namespace godot;

class InputManager
{
public:
    InputManager() { };
    ~InputManager();
    void Init(Node* root, NetworkManager& networkManager, CheckpointList<TransformComponent>& transforms);
    void Loop(const Vector2& mousePos);
private:
    void BuildTower(const Vector2& mousePos);
private:
    NetworkManager* m_networkManager;

    CheckpointList<TransformComponent>* m_transforms;

    Node* m_root;
};

#endif