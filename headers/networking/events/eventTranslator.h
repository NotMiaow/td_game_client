#ifndef EVENT_TRANSLATOR_H__
#define EVENT_TRANSLATOR_H__

#include <string>
#include <vector>

//Libraries
#include "basicLib.h"
#include "checkpointList.h"

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
	PlayerComponent player;
	BankComponent bank;
	std::vector<PlayerComponent>* players = new std::vector<PlayerComponent>();;
	std::vector<BankComponent>* banks = new std::vector<BankComponent>();;

	if (!ToInt(elements[0], playerPosition)) return CreateErrorEvent(EReadyUp, GEWrongParameterType);
	for (int i = 1; i < elements.size(); i++)
	{
		//Player
		player.connected = elements[i] == "1";
		player.ready = elements[++i] == "1";
		if(ToInt(elements[++i], player.lives)) players->push_back(player);
		//Banks
		if(ToInt(elements[++i], bank.gold) && ToInt(elements[++i], bank.income)) banks->push_back(bank);
		else return CreateErrorEvent(EReadyUp, GEWrongParameterType);
	}

	Event *e = new ReadyUpEvent(playerPosition, players, banks);
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