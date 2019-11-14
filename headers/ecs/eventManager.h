#ifndef EVENT_HANDLER_H__
#define EVENT_HANDLER_H__

//Godot includes
#include "Vector2.hpp"

//Components
#include "motorComponent.h"
#include "transformComponent.h"

//Required
#include <iostream>
#include <math.h>
#include "checkpointList.h"

class EventManager
{
public:
    EventManager() { }
    EventManager(CheckpointList<MotorComponent>& motors, CheckpointList<TransformComponent>& transforms);
    ~EventManager();
private:

private:
   	CheckpointList<MotorComponent>* m_motors;
	CheckpointList<TransformComponent>* m_transforms;
};

#endif
