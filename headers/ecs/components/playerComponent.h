#ifndef PLAYER_COMPONENT_H__
#define	PLAYER_COMPONENT_H__

#include "client.h"

struct PlayerComponent
{
	bool connected;
	bool ready;
	int lives;
};

#endif
