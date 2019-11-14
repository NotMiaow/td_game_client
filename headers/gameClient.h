#ifndef GAME_CLIENT_HPP
#define GAME_CLIENT_HPP

#include <iostream>
#include <Godot.hpp>
#include <Node.hpp>

namespace godot {

class GameClient : public Node {
	GODOT_CLASS(GameClient, Node)

private:
public:
	static void _register_methods();
	void _init();

	void Iteration();
};

}

#endif