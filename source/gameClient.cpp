#include "gameClient.h"

using namespace godot;

void GameClient::_register_methods()
{
	register_method("Iteration", &GameClient::Iteration);

}

void GameClient::_init()
{

}

void GameClient::Iteration()
{
	Godot::print("hello");
}