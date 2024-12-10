#pragma once

#include "../GameConsole.h"

typedef enum
{
    Ping_Start_Game = 1,
    Ping_Move_Step = 2,
    Ping_GAME_OVER = 3,
} PingState;

struct Ball
{
    float x, y;
    float dx, dy;
    float bounciness;
};
extern PingState PING_CURRENT_STATE;
PingState
PingUpdateFSM(PingState curState, struct Joystick_input Joystick_input);
void playPing(struct Joystick_input joystickInput, float deltaTime);
void displayGameOver();
void CollideBall();