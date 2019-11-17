#ifndef CST_H__
#define CST_H__

#include <string>

static const int TOKEN_LENGTH = 8;
static const int MAX_PLAYERS = 4;

//Engine constants
enum Behaviour { Move, Rage, Despawn };
const int MAX_GROUPS_ON_GRID = 9;
const int SPAWN_SIZE = 4;
const int GRID_SIZE_X = 11;
const int GRID_SIZE_Y = 11;
const int DESPAWN_SIZE = 4;
const int STARTING_GOLD = 10000;
const int POST_GAME_DURATION = 1; //Milliseconds

//Networking
const int RECONNECTION_ATTEMPT_DELAY = 4000;

//Networking debugging needs
const int PLAYER_ID = 45; //{ 45, 120, 645, 356 };
const std::string PLAYER_TOKEN = "e2S5Ert6"; //{ "e2S5Ert6", "cy53rt0u", "3lc4vu12", "f4C7hagj" };
#endif