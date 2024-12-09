#pragma once
#include "../utils.h"
#include "../GameConsole.h"

typedef enum
{
    Ping_Start_Game = 1,
    Ping_Move_Step = 2,
    Ping_GAME_OVER = 3,
} PingState;

// Structures
struct Ball
{
    float x, y;
    float dx, dy;
    float bounciness;
};

PingState PingUpdateFSM(PingState curState, Joystick_input Joystick_input);
void playPing(Joystick_input joystickInput, float deltaTime);
void displayGameOver();
void CollideBall();
bool checkGameOver();