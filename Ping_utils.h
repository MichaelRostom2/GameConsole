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
PingState PingUpdateFSM(PingState curState, struct Joystick_input Joystick_input);
void playPing(struct Joystick_input joystickInput, float deltaTime);
void displayGameOver();
void updateBall(Ball &ball, float &Gravity);
void CollideBall(Ball &ball, int &Paddle_y_pos);
void DrawPingIntro();
void horizontalBounce(Ball &ball);
int updatePaddle(Joystick_input Joystick_input, int Paddle_y_pos);