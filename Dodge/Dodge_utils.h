#pragma once
#include "../utils.h"
#include "../GameConsole.h"

typedef enum
{
    Dodge_Start_Game = 1,
    Dodge_Move_Step = 2,
    Dodge_GAME_OVER = 3,
} DodgeState;

DodgeState DodgeUpdateFSM(DodgeState curState, Joystick_input Joystick_input);
void playDodge(Joystick_input Joystick_input);
void displayDodgeGameOver();
void incrementDodgeScore();