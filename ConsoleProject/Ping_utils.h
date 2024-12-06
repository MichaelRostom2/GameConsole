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

PingState PingUpdateFSM(PingState curState, Joystick_input Joystick_input);
void drawBackWall(Arduino_GFX *gfx, int screenWidth, int screenHeight);
void drawPaddle(Arduino_GFX *gfx, int y);
void drawBall(Arduino_GFX *gfx, int x, int y);
void playPing(Joystick_input joystickInput);
