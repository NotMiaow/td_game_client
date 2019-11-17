#ifndef ACTION_H__
#define ACTION_H__

#include <iostream>
#include <string>
#include <vector>
#include <string>
#include <sstream>

#include "sharedLanguage.h"
#include "actionLanguage.h"

struct Action
{
	virtual ActionType GetType() const = 0;
	virtual std::string ToNetworkable() const = 0;

	int clientId;
	std::string sessionToken;
};

struct ErrorAction : public Action
{
	//Read only
	ErrorAction(ActionType aType, NetworkingErrorType neType)
	{
		this->aType = aType;
		this->neType = neType;
	}
	ActionType GetType() const { return AError; }
	std::string ToNetworkable() const
	{
		std::ostringstream os;
		os << "{" << AError << ";" << aType << ";" << neType << "}";
		return os.str();
	}

	ActionType aType;
	NetworkingErrorType neType;
};

struct ConnectAction : public Action
{
	ConnectAction()	{ }
	ActionType GetType() const { return AConnect; }
	std::string ToNetworkable() const
	{
		std::ostringstream os;
		os << "{" << AConnect << ";" << clientId << ";" << sessionToken << "}";
		return os.str();
	}
};

struct DisconnectAction : public Action
{
	DisconnectAction() { }
	DisconnectAction(DisconnectReason reason)
	{
		this->reason = reason;
	}
	ActionType GetType() const { return ADisconnect; }
	std::string ToNetworkable() const
	{
		std::ostringstream os;
		os << "{" << ADisconnect << ";" << reason << "}";
		return os.str();
	}

	DisconnectReason reason;
};

struct GameAction : public Action
{
	GameAction() { }
	GameAction(const std::string& gameEvent)
	{
		this->gameEvent = gameEvent;
	}
	ActionType GetType() const { return AGameAction; }
	std::string ToNetworkable() const
	{
		std::ostringstream os;
		os << "{" << AGameAction << ";" << clientId << ";" << sessionToken << ";" << gameEvent << "}" <<std::endl;
		return os.str();
	}

	std::string gameEvent;
};

#endif