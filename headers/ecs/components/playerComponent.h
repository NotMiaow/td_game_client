#ifndef PLAYER_COMPONENT_H__
#define	PLAYER_COMPONENT_H__

#include "client.h"

struct PlayerComponent
{
	PlayerComponent() : connected(false), ready(false), lives(0) { }
	PlayerComponent(const bool& connected, const bool& ready, const int& lives) { }
	Client* client;
	bool connected;
	bool ready;
	int lives;
};

#endif
