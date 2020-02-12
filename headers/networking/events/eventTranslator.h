#ifndef EVENT_TRANSLATOR_H__
#define EVENT_TRANSLATOR_H__

#include <string>
#include <vector>
#include <deque>

//Libraries
#include "basicLib.h"

//Godot
#include <Godot.hpp>
#include <Vector2.hpp>

//Networking
#include "event.h"
#include "eventLanguage.h"

//Game related
#include "gameEnums.h"

//Components
#include "bankComponent.h"
#include "playerComponent.h"

using namespace godot;

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

static Event *CreateReadyUpEvent(const std::vector<std::string>& elements)
{
	int playerId;
	int entityId;
	PlayerComponent player;
	BankComponent bank;
	std::vector<int>* entityIds = new std::vector<int>();
	std::vector<PlayerComponent>* players = new std::vector<PlayerComponent>(); 
	std::vector<BankComponent>* banks = new std::vector<BankComponent>();

	if (!ToInt(elements[0], playerId)) return CreateErrorEvent(EReadyUp, GEWrongParameterType);
	for (int i = 1; i < elements.size(); i++)
	{
		player.connected = elements[i++] == "1";
		player.ready = elements[i++] == "1";
		if( ToInt(elements[i++], entityId) &&
			ToInt(elements[i++], player.lives) &&
			ToInt(elements[i++], bank.gold) &&
			ToInt(elements[i], bank.income))
		{
			entityIds->push_back(entityId);
			players->push_back(player);
			banks->push_back(bank);
		}
		else return CreateErrorEvent(EReadyUp, GEWrongParameterType);
	}

	Event *e = new ReadyUpEvent(playerId, entityIds, players, banks);
	return e;
}

static Event* CreateSpawnUnitGroupEvent(const std::vector<std::string>& elements)
{
	int playerId, entityId;
	if(!(ToInt(elements[0], playerId) && ToInt(elements[1], entityId))) return CreateErrorEvent(ESpawnUnitGroup, GEWrongParameterType);

	Event* e = new SpawnUnitGroupEvent(playerId, entityId);
	return e;
}

static Event* CreateNewPathEvent(const std::vector<std::string>& elements)
{
	int playerId;
	int entityId;
	std::deque<Vector2>* path = new std::deque<Vector2>();

	if (elements.size() != 3)
		return CreateErrorEvent(ENewPath, GEWrongParemeterAmount);
	if (!ToInt(elements[0], playerId))
		return CreateErrorEvent(ENewPath, GEWrongParameterType);
	if (!ToInt(elements[1], entityId))
		return CreateErrorEvent(ENewPath, GEWrongParameterType);
	if (!ToPath(elements[2], *path))
		return CreateErrorEvent(ENewPath, GEWrongParameterType);

	Event* e = new NewPathEvent(playerId, entityId, path);
	return e;
}

static Event* CreateRageEvent(const std::vector<std::string>& elements)
{
	int playerId;
	int entityId;

	if (elements.size() != 2)
		return CreateErrorEvent(ERage, GEWrongParemeterAmount);
	if (!(ToInt(elements[0], playerId) && ToInt(elements[1], entityId)))
		return CreateErrorEvent(ERage, GEWrongParameterType);
	
	Event* e = new RageEvent(playerId, entityId);
	return e;
}

static Event *CreateBuildTowerEvent(const std::vector<std::string>& elements)
{
	int playerId, remainingGold, entityId, towerType;
	Vector2 position;

	if (elements.size() != 5) return CreateErrorEvent(EBuildTower, GEWrongParemeterAmount);
	if (!(ToInt(elements[0], playerId) && ToInt(elements[1], remainingGold) &&
		ToInt(elements[2], entityId) &&  ToInt(elements[3], towerType) &&
		ToPosition(elements[4], position)))
		return CreateErrorEvent(EBuildTower, GEWrongParameterType);

	Event *e = new BuildTowerEvent(playerId, entityId, remainingGold, towerType, position);
	return e;
}

static Event *CreateSellTowerEvent(const std::vector<std::string>& elements)
{
	int playerId, entityId, remainingGold;

	if (elements.size() != 3) return CreateErrorEvent(ESellTower, GEWrongParemeterAmount);
	if (!(ToInt(elements[0], playerId) && ToInt(elements[1], entityId) &&
		ToInt(elements[2], remainingGold)))
		return CreateErrorEvent(ESellTower, GEWrongParameterType);

	Event *e = new SellTowerEvent(playerId, entityId, remainingGold);
	return e;
}

static Event *CreateSendUnitGroupEvent(const std::vector<std::string>& elements)
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
			return CreateSpawnUnitGroupEvent(elements);
		case ENewPath:
			return CreateNewPathEvent(elements);
		case ERage:
			return CreateRageEvent(elements);
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