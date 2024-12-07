#pragma once
#include "utils.h"
#include "GameConsole.h"

typedef enum
{
    sStart_Game = 1,
    sMove_Step = 2,
    sCollision_Step = 3,
    sGAME_OVER = 4,
} PingState;

// Structures
struct Ball
{
    int x, y;
    int dx, dy;
    int bounciness;
    bool gravity_right;
};

PingState PingUpdateFSM(PingState curState, Joystick_input Joystick_input);
void playPing(Joystick_input joystickInput);
