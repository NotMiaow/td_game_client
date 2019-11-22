#ifndef INPUT_MANAGER_H__
#define INPUT_MANAGER_H__

//Godot
#include <Godot.hpp>
#include <Viewport.hpp>
#include <Node.hpp>
#include <Camera.hpp>
#include <String.hpp>
#include <Input.hpp>
#include <Vector2.hpp>
#include <Vector3.hpp>
#include <PhysicsDirectSpaceState.hpp>
#include <Dictionary.hpp>
#include <RID.hpp>
#include <KinematicBody.hpp>
#include <RayCast.hpp>
#include <World.hpp>
#include <Ref.hpp>
#include <ResourceLoader.hpp>
#include <PackedScene.hpp>
#include <SceneTree.hpp>
#include <Transform.hpp>
#include <RigidBody.hpp>
#include <string>

//ECS
#include "event.h"
#include "shared_queue.h"

#include "cst.h"

using namespace godot;

class InputManager
{
public:
    InputManager() { };
    ~InputManager();
    void Init(Node* root, SharedQueue<Event*>& eventQueue);
    void Loop(Vector2 mousePos);
private:
    SharedQueue<Event*>* m_eventQueue;

    Node* m_root;
};

#endif