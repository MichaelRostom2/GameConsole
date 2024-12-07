#include "utils.h"

typedef enum
{
    sStart_Game = 1,
    sMove_Step = 2,
    sCollision_Step = 3,
    sGAME_OVER = 4,
} DodgeState;

// DodgeState DodgeUpdateFSM(Arduino_GFX *gfx, DodgeState curState, Joystick_input Joystick_input);
// void playDodge(Joystick_input Joystick_input);
