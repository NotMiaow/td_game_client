#ifndef CST_H__
#define CST_H__

#include <string>

//Game
const int MAX_CLIENTS = 4;
enum Behaviour { WaitingForPath, Move, Rage, Despawn };
const int MAX_GROUPS_ON_GRID = 9;
const int SPAWN_SIZE = 4;
const int GRID_SIZE_X = 11;
const int GRID_SIZE_Y = 11;
const int DESPAWN_SIZE = 4;
const int SPAWN_POSITION_Y = -4;
const int SPAWN_POSITION_X = 5;

//Networking
const int BUFFER_LENGTH = 512;
const int RECONNECTION_ATTEMPT_DELAY = 4000;

//ECS constants
enum PlayerCheckpoints { PLAYER_PLAYERS = 0, PLAYER_CHECKPOINT_SIZE };
enum BankCheckpoints { PLAYER_BANKS = 0, BANK_CHECKPOINT_SIZE };
enum TransformsCheckpoint { UNIT_GROUP_TRANSFORMS = 0, TOWER_TRANSFORMS, TRANSFORM_CHECKPOINT_SIZE };
enum MotorCheckpoints { UNIT_GROUP_MOTORS = 0, MOTOR_CHECKPOINT_SIZE };
enum OffenseCheckpointss { UNIT_GROUP_OFFENSES = 0, TOWER_OFFENSES, OFFENSE_CHECKPOINT_SIZE };

enum ComponentTypes { TPlayer, TBank, TMotor, TOffense, TTransform };
static int GetCheckpoint(const int& playerIndex, const ComponentTypes& componentType, const int& checkpointIndex)
{
    switch (componentType)
    {
    case TPlayer:    return playerIndex * PLAYER_CHECKPOINT_SIZE + checkpointIndex;
    case TBank:      return playerIndex * BANK_CHECKPOINT_SIZE + checkpointIndex;
    case TMotor:     return playerIndex * MOTOR_CHECKPOINT_SIZE + checkpointIndex;
    case TOffense:   return playerIndex * OFFENSE_CHECKPOINT_SIZE + checkpointIndex;
    case TTransform: return playerIndex * TRANSFORM_CHECKPOINT_SIZE + checkpointIndex;
    default:        return -1;
    }
}

//Networking debugging needs
const int PLAYER_ID = 45; //{ 45, 120, 645, 356 };
const std::string PLAYER_TOKEN = "e2S5Ert6"; //{ "e2S5Ert6", "cy53rt0u", "3lc4vu12", "f4C7hagj" };
#endif