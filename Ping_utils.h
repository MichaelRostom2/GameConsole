#pragma once

#include "GameConsole.h"

typedef enum
{
    PING_START = 1,
    PING_MOVE = 2,
    PING_GAME_OVER = 3,
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
void DrawPingIntro();