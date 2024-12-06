// #include "Dodge_utils.h"

// /*!
//   @brief  Updates Dodge according to FSM states and guards
//   @param  gfx  Pointer to Arduino_GFX screen object
//   @param  curState Current state of Game
//   @param  mils Current time in ms.
//   @param  Joystick_input Latest input from Joystick
//   @return New dodge State
// */
// DodgeState DodgeUpdateFSM(Arduino_GFX *gfx, DodgeState curState, long mils, Joystick_input Joystick_input)
// {
//     DodgeState nextState;
//     switch (curState)
//     {
//     case sStart_Game:
//         // drawPlayer()
//         break;

//     case sMove_Step:
//         updateBullets();
//         if (checkCollisions())
//         {
//             nextState = sGAME_OVER;
//         }
//         else
//         {
//             nextState = sMove_Step;
//         }
//         break;
//     case sGAME_OVER:
//         // DisplayGameOver
//         break;
//     }
//     return nextState;
// }
// DodgeState DODGE_CURRENT_STATE = sStart_Game;
// void playDodge(Joystick_input Joystick_input)
// {
// }