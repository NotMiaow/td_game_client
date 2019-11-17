#ifndef GAME_TRANSLATOR_H__
#define GAME_TRANSLATOR_H__

//Godot includes
#include <Vector2.hpp>

//required
#include <string>
#include "basicLib.h"
#include "event.h"
#include "eventLanguage.h"
#include "gameEnums.h"
#include "queue.h"

static Event *CreateErrorEvent(EventType aType, GameErrorType eType)
{
	Event *e = new ErrorEvent(aType, eType);
	return e;
}

static Event *CreateConnectEvent()
{
	Event *e = new ConnectEvent();
	return e;
}

static Event *CreateDisconnectEvent(DisconnectReason reason = RKicked)
{
	Event *e = new DisconnectEvent(reason);
	return e;
}

static Event *CreateReadyUpEvent(std::vector<std::string> elements)
{
	int playerPosition;
	Queue<ResourceComponent> resources;

	if (!ToInt(elements[0], playerPosition)) return CreateErrorEvent(EBuildTower, GEWrongParameterType);
	for (int i = 1; i < elements.size(); i++)
	{
		ResourceComponent resource;
		if(ToInt(elements[i], resource.gold)) resources.Push(resource);
		else return CreateErrorEvent(EReadyUp, GEWrongParameterType);
	}

	Event *e = new ReadyUpEvent(playerPosition, resources);
	return e;
}

static Event *CreateBuildTowerEvent(std::vector<std::string> elements)
{
	int towerType;
	godot::Vector2 position;

	if (elements.size() != 2)
		return CreateErrorEvent(EBuildTower, GEWrongParemeterAmount);
	if (!ToInt(elements[0], towerType))
		return CreateErrorEvent(EBuildTower, GEWrongParameterType);
	if (!ToPosition(elements[1], position))
		return CreateErrorEvent(EBuildTower, GEWrongParameterType);

	Event *e = new BuildTowerEvent(towerType, position);
	return e;
}

static Event *CreateSellTowerEvent(std::vector<std::string> elements)
{
	int towerPosition;

	if (elements.size() != 1)
		return CreateErrorEvent(ESellTower, GEWrongParemeterAmount);
	if (!ToInt(elements[0], towerPosition))
		return CreateErrorEvent(ESellTower, GEWrongParameterType);

	Event *e = new SellTowerEvent(towerPosition);
	return e;
}

static Event *CreateSendUnitGroupEvent(std::vector<std::string> elements)
{
	int unitType;

	if (elements.size() != 1)
		return CreateErrorEvent(ESendUnitGroup, GEWrongParemeterAmount);
	if (!ToInt(elements[0], unitType))
		return CreateErrorEvent(ESendUnitGroup, GEWrongParameterType);

	Event *e = new SendUnitGroupEvent(unitType);
	return e;
}

static Event *CreateGameEvent(std::vector<std::string> elements)
{
	int eventType = -1;
	bool canInt = ToInt(elements[0], eventType);

	elements.erase(elements.begin());

	if (canInt)
	{
		switch (eventType)
		{
		case EConnect:
			return CreateConnectEvent();
		case EDisconnect:
			return CreateDisconnectEvent();
		case EReadyUp:
			return CreateReadyUpEvent(elements);
		case ESpawnUnitGroup:
			return CreateSendUnitGroupEvent(elements);
			break;
		case ENewPath:
			//readonly
			break;
		case ERage:
			//readonly
			break;
		case EBuildTower:
			return CreateBuildTowerEvent(elements);
		case ESellTower:
			return CreateSellTowerEvent(elements);
		case ESendUnitGroup:
			return CreateSendUnitGroupEvent(elements);
		default:
			return CreateErrorEvent(ECreateEvent, GEWrongEventType);
		}
	}
	return CreateErrorEvent(ECreateEvent, GEWrongEventType);
}

#endif