#include "inputManager.h"

using namespace godot;

InputManager::~InputManager()
{

}

void InputManager::Init(Node* root, SharedQueue<Event*>& eventQueue)
{
    m_eventQueue = &eventQueue;
    m_root = root;
}

void InputManager::Loop(Vector2 mousePos)
{
    if(Input::get_singleton()->is_action_just_released(String("mouse_1")) && !(mousePos.x == 0 && mousePos.y == 0))
    {
        if(mousePos.x < GRID_SIZE_X && mousePos.x > 0 && mousePos.y < GRID_SIZE_Y && mousePos.y > 0)
        {
            Godot::print(std::string("(" + std::to_string(mousePos.x) + ":" + std::to_string(mousePos.y) + ")").c_str());
            ResourceLoader* ressourceLoader = ResourceLoader::get_singleton();
            Ref<PackedScene> packedScene = ressourceLoader->load("res://tower.tscn");
            RigidBody* tower = (RigidBody*)packedScene->instance();
            m_root->add_child(tower);
            const Vector3 position(5,0,5);
            tower->global_translate(position);
        } 
    }
}

