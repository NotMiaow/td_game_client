#ifndef NETWORKING_TRANSLATOR_H__
#define NETWORKING_TRANSLATOR_H__

#include <iostream>
#include <string>
#include <vector>

#include "basicLib.h"
#include "action.h"
#include "actionLanguage.h"

static Action* CreateErrorAction(ActionType aType, NetworkingErrorType eType)
{
	Action* action = new ErrorAction(aType, eType);
	return action;
}

static Action* CreateConnectAction(const std::vector<std::string>& elements)
{
	if (elements.size() != 0) return CreateErrorAction(AConnect, NEWrongParemeterAmount);

	Action *action = new ConnectAction();
	return action;
}

static Action* CreateDisconnectAction(DisconnectReason reason = RKicked)
{
	Action *action = new DisconnectAction(reason);
	return action;
}

static Action* CreateGameAction(std::vector<std::string> elements)
{
	if (elements.size() != 1) return CreateErrorAction(AConnect, NEWrongParemeterAmount);

	Action *action = new GameAction(elements[0]);
	return action;
}


static Action* CreateAction(std::vector<std::string> elements)
{
	if (elements.size() < 1) return CreateErrorAction(ACreateAction, NEWrongParemeterAmount);

	int actionType = -1;
	bool canInt = ToInt(elements[0], actionType);
	elements.erase(elements.begin());

	if (canInt)
	{
		switch (actionType)
		{
		case AConnect:
			return CreateConnectAction(elements);
		case ADisconnect:
			return CreateDisconnectAction();
		case AGameAction:
			return CreateGameAction(elements);
		default:
			return CreateErrorAction(ACreateAction, NEWrongActionType);
		}
	}
	return CreateErrorAction(ACreateAction, NEWrongActionType);
}

#endif