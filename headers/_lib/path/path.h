#ifndef PATH_H__
#define	PATH_H__

#include <Vector2.hpp>

#include "queue.h"

using namespace godot;

struct Path
{
	Queue<Vector2>* path;
	int length;
	int lastYDirection;
	int lastXDirection;
};

#endif