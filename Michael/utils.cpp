#include "utils.h"

// Joystick offsets so that starting position gives x=0, y=0. (manual adjust accordingly)
const int JOYSTICK_X_OFFSET = 501;
const int JOYSTICK_Y_OFFSET = 504;

/*!
    @brief  reads the analogue values of x and y from the joystick
    @return  Joystick_input struct
*/
Joystick_input pollInputs()
{
  // Create a Joystick_input structure
  Joystick_input joystickInput;

  // Read raw analog values and adjust by offsets
  joystickInput.x = analogRead(JOYSTICK_VRX_PIN) - JOYSTICK_X_OFFSET;
  joystickInput.y = analogRead(JOYSTICK_VRY_PIN) - JOYSTICK_Y_OFFSET;
  return joystickInput;
}
/*!
  @brief  Updates console according to FSM states and guards
  @param  gfx  Pointer to Arduino_GFX screen object
  @param  curState Current state of Game
  @param  Joystick_input Latest input from Joystick
  @return New console State
*/
ConsoleState DodgeUpdateFSM(Arduino_GFX *gfx, ConsoleState curState, Joystick_input Joystick_input)
{
  ConsoleState nextState;
  switch (curState)
  {
  case sStartup_Step:
    // draw Menu
    break;

  case sDisplay_Menu:
    if (Joystick_input.x < -250)
    {
      switchToPing();
    }
    else if (Joystick_input.x < 250)
    {
      switchToDodge();
    }
    break;
  }
  return nextState;
}