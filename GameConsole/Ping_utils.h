#pragma once
#include "utils.h"
#include "GameConsole.h"

// TODO: convert to PingStartGame
typedef enum
{
    sStart_Game = 1,
    sMove_Step = 2,
    sGAME_OVER = 3,
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