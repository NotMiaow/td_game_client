#ifndef EVENT_H__
#define	EVENT_H__

#include <iostream>
#include <string>
#include <vector>
#include <string>
#include <sstream>
#include <deque>

//Libraries
#include "basicLib.h"

//Godot includes
#include "Godot.hpp"
#include "Vector2.hpp"

//Networking
#include "sharedLanguage.h"
#include "eventLanguage.h"

//Components
#include "playerComponent.h"
#include "bankComponent.h"

using namespace godot;

struct Event
{
	virtual EventType GetType() const = 0;
	virtual std::string ToNetworkable() const = 0;

	int playerId;
};

struct ErrorEvent : public Event
{
	ErrorEvent(EventType eType, GameErrorType geType)
	{
		this->eType = eType;
		this->geType = geType;
	}
	EventType GetType() const { return EError; }
	std::string ToNetworkable() const { return ""; }

	EventType eType;
	GameErrorType geType;
};

struct ConnectEvent : public Event
{
	ConnectEvent() { }
	EventType GetType() const { return EConnect; }
	std::string ToNetworkable() const
	{
		std::ostringstream os;
		os << "{" << EConnect << "}";
		return os.str();
	}
};

struct DisconnectEvent : public Event
{
	DisconnectEvent() { }
	DisconnectEvent(DisconnectReason reason)
	{
		this->reason = reason;
	}
	EventType GetType() const { return EDisconnect; }
	std::string ToNetworkable() const
	{
		std::ostringstream os;
		os << "{" << EDisconnect << ";" << reason << "}";
		return os.str();
	}

	DisconnectReason reason;
};

struct ReadyUpEvent : public Event
{
	ReadyUpEvent() { entityIds = 0; players = 0; banks = 0; }
	ReadyUpEvent(const int& playerId, std::vector<int>* entityIds, std::vector<PlayerComponent>* players, std::vector<BankComponent>* banks)
	{ 
		this->playerId = playerId;
		this->entityIds = entityIds;
		this->players = players;
		this->banks = banks;
	}
	~ReadyUpEvent() { delete entityIds; delete players; delete banks; }
	EventType GetType() const { return EReadyUp; }
	std::string ToNetworkable() const
	{
		std::ostringstream os;
		os << "{" << EReadyUp << "}";
		return os.str();
	}

	std::vector<int>* entityIds;
	std::vector<PlayerComponent>* players;
	std::vector<BankComponent>* banks;
};

struct SpawnUnitGroupEvent : public Event
{
	SpawnUnitGroupEvent() { }
	SpawnUnitGroupEvent(const int& playerId, const int& entityId)
	{
		this->playerId = playerId;
		this->entityId = entityId;
	}
	EventType GetType() const { return ESpawnUnitGroup; }
	std::string ToNetworkable() const
	{
		std::ostringstream os;
		os << "{" << ESpawnUnitGroup << "}";
		return os.str();
	}

	int entityId;
};

struct NewPathEvent : public Event
{
	NewPathEvent(const int& playerId, const int& entityId, std::deque<Vector2>* path)
	{
		this->playerId = playerId;
		this->entityId = entityId;
		this->path = path;
	}
	~NewPathEvent() { delete path; }
	EventType GetType() const { return ENewPath; }
	std::string ToNetworkable() const
	{
		std::ostringstream os;
		os << "{" << ENewPath << "}";
		return os.str();
	}

	int entityId;
	std::deque<Vector2>* path;
};

struct RageEvent : public Event
{
	RageEvent() { }
	RageEvent(const int& playerId, const int& entityId)
	{
		this->playerId = playerId;
		this->entityId = entityId;
	}
	EventType GetType() const { return ERage; }
	std::string ToNetworkable() const
	{
		std::ostringstream os;
		os << "{" << ERage << ";" << entityId << "}";
		return os.str();
	}

	int entityId;
};

struct BuildTowerEvent : public Event
{
	BuildTowerEvent() { }
	BuildTowerEvent(const int& playerId, const int& entityId, const int& remainingGold, const int& towerType, const Vector2& position)
	{
		this->playerId = playerId;
		this->entityId = entityId;
		this->remainingGold = remainingGold;
		this->towerType = towerType;
		this->position = position;
	}
	EventType GetType() const { return EBuildTower; }
	std::string ToNetworkable() const
	{
		std::ostringstream os;
		os << "{" << EBuildTower << ";" << towerType << ";(" << position.y << ":" << position.x << ")}";
		return os.str();
	}

	int entityId;
	int remainingGold;
	int towerType;
	Vector2 position;
};

struct SellTowerEvent : public Event
{
	SellTowerEvent() { }
	SellTowerEvent(const int& playerId, const int& entityId, const int& remainingGold)
	{
		this->playerId = playerId;
		this->entityId = entityId;
		this->remainingGold = remainingGold;
	}
	EventType GetType() const { return ESellTower; }
	std::string ToNetworkable() const
	{
		std::ostringstream os;
		os << "{" << ESellTower << ';' << entityId << '}';
		return os.str();
	}

	int entityId;
	int remainingGold;
};

struct SendUnitGroupEvent : public Event
{
	SendUnitGroupEvent() { }
	SendUnitGroupEvent(const int& unitType)
	{
		this->unitType = unitType;
	}
	EventType GetType() const { return ESendUnitGroup; }
	std::string ToNetworkable() const
	{
		std::ostringstream os;
		os << "{" << ESendUnitGroup << "}";
		return os.str();
	}

	int unitType;
};

#endif
