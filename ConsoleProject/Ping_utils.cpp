#include "Ping_utils.h"

// Game drawing variables
const int BackWallThickness = 40;
const int PaddleWidth = 40;
const int PaddleHeight = 200;
const int BallRadius = 30;

/*!
  @brief  Draws the back wall of Ping Game. The back wall is shaped like a U rotated counter-clockwise
  @param  gfx               Pointer to Arduino_GFX screen object
  @param  screenWidth       Width of screen
  @param  screenHeight      Height of screen
*/
void drawBackWall(Arduino_GFX *gfx, int screenWidth, int screenHeight)
{
  //  Draw the verticle back wall part
  gfx->fillRect(screenWidth - BackWallThickness, 0, BackWallThickness, screenHeight, WHITE);

  // Draw the top and bottom horizontal part
  gfx->fillRect((screenWidth / 2), 0, (screenWidth / 2) - BackWallThickness, BackWallThickness, WHITE);
  gfx->fillRect((screenWidth / 2), screenHeight, (screenWidth / 2) - BackWallThickness, BackWallThickness, WHITE);
}

/*!
  @brief  Draws the paddle of Ping Game.
  @param  gfx    Pointer to Arduino_GFX screen object
  @param  y      Top-Left y coordinate of paddle
*/
void drawPaddle(Arduino_GFX *gfx, int y)
{
  // Draw the verticle back wall part
  gfx->fillRect(0, y, PaddleWidth, PaddleHeight, WHITE);
}

/*!
  @brief  Draws the ball of Ping Game.
  @param  gfx    Pointer to Arduino_GFX screen object
  @param  x      Center-point x coordinate
  @param  y      Center-point y coordinate
*/
void drawBall(Arduino_GFX *gfx, int x, int y)
{
  gfx->fillCircle(x, y, BallRadius, WHITE);
}

/*!
  @brief  Updates Ping according to FSM states and guards
  @param  gfx  Pointer to Arduino_GFX screen object
  @param  curState Current state of Game
  @param  mils Current time in ms.
  @param  Joystick_input Latest input from Joystick
  @return New Ping State
*/
PingState PingUpdateFSM(Arduino_GFX *gfx, PingState curState, long mils, Joystick_input Joystick_input)
{
  PingState nextState;
  switch (curState)
  {
  case sStart_Game:
    drawBackWall(gfx, 240, 320);
    drawPaddle(gfx, 30);
    drawBall(gfx, 120, 160);
    nextState = sMove_Step;
    break;

  case sMove_Step:
    updateBall();
    if (checkCollision())
    {
      nextState = sCollision_Step;
    }
    else
    {
      nextState = sMove_Step;
    }
    break;

  case sCollision_Step:
    if (x < x_paddle)
    {
      nextState = sGAME_OVER;
    }
    else
    {
      nextState = sMove_Step;
    }
    break;
  case sGAME_OVER:
    // DisplayGameOver
    break;
  }
  return nextState;
}