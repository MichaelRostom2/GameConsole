#include "utils.h"

typedef enum
{
    sStart_Game = 1,
    sMove_Step = 2,
    sCollision_Step = 3,
    sGAME_OVER = 4,
} PingState;

PingState PingUpdateFSM(Arduino_GFX *gfx, PingState curState, long mils, Joystick_input Joystick_input);
void drawBackWall(Arduino_GFX *gfx, int screenWidth, int screenHeight);
void drawPaddle(Arduino_GFX *gfx, int y);
void drawBall(Arduino_GFX *gfx, int x, int y);
