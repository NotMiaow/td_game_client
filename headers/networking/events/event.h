#ifndef EVENT_H__
#define	EVENT_H__

#include <iostream>
#include <string>
#include <vector>
#include <string>
#include <sstream>

//Godot includes
#include "Vector2.hpp"

#include "checkpointList.h"
#include "queue.h"
#include "resourceComponent.h"
#include "basicLib.h"
#include "sharedLanguage.h"
#include "eventLanguage.h"

struct Event
{
	virtual EventType GetType() const = 0;
	virtual std::string ToNetworkable() const = 0;

	int clientId;
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
	ReadyUpEvent() { }
	ReadyUpEvent(int playerPosition, Queue<ResourceComponent> resources)
	{
		this->playerPosition = playerPosition;
		this->resources = resources;
	}
	EventType GetType() const { return EReadyUp; }
	std::string ToNetworkable() const
	{
		std::ostringstream os;
		os << "{" << EReadyUp << "}";
		return os.str();
	}

	int playerPosition;
	Queue<ResourceComponent> resources;
};

struct SpawnUnitGroupEvent : public Event
{
	SpawnUnitGroupEvent() { }
	EventType GetType() const { return ESpawnUnitGroup; }
	std::string ToNetworkable() const
	{
		std::ostringstream os;
		os << "{" << ESpawnUnitGroup << "}";
		return os.str();
	}
};

struct NewPathEvent : public Event
{
	NewPathEvent() { }
	EventType GetType() const { return ENewPath; }

	int motorPosition;
	Queue<godot::Vector2>* path;
};

struct RageEvent : public Event
{
	RageEvent() { }
	EventType GetType() const { return ERage; }
	std::string ToNetworkable() const
	{
		std::ostringstream os;
		os << "{" << ERage << ";" << motorPosition << "}";
		return os.str();
	}

	int motorPosition;
};

struct BuildTowerEvent : public Event
{
	BuildTowerEvent(const int& towerType, const godot::Vector2& position)
	{

		this->clientId = clientId;
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

	int towerType;
	godot::Vector2 position;
};

struct SellTowerEvent : public Event
{
	SellTowerEvent(const int& towerPosition)
	{

		this->clientId = clientId;
		this->towerPosition = towerPosition;
	}
	EventType GetType() const { return ESellTower; }
	std::string ToNetworkable() const
	{
		std::ostringstream os;
		os << "{" << ESellTower << ";" << towerPosition << ";" << sellValue << "}";
		return os.str();
	}

	int towerPosition;
	int sellValue;
};

struct SendUnitGroupEvent : public Event
{
	SendUnitGroupEvent(const int& unitType)
	{
		this->clientId = clientId;
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
