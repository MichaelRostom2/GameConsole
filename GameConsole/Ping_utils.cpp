#include "Ping_utils.h"

// Game drawing variables
const int BackWallThickness = 8;
const int PaddleWidth = 8;
const int PaddleHeight = 64;
const int BallRadius = 8;
const int Gravity = 5;

// Ping tracking variables
Ball ball = Ball{120, 160, 1, 1, 0, false};
int Paddle_y_pos = (screenHeight / 2) - (PaddleHeight / 2);

PingState PING_CURRENT_STATE = sStart_Game;

/*!
  @brief  Plays the Ping Game by calling updateFSM of Ping
  @param  joystickInput Latest input from Joystick
*/
void playPing(Joystick_input joystickInput)
{
  PING_CURRENT_STATE = PingUpdateFSM(PING_CURRENT_STATE, joystickInput);
}

/*!
  @brief  Draws the back wall of Ping Game.
*/
void drawBackWall()
{
  //  Draw the verticle back wall part
  gfx->fillRect(screenWidth - BackWallThickness, 0, BackWallThickness, screenHeight, WHITE);
}

/*!
  @brief  Draws the paddle of Ping Game.
  @param  y      Top-Left y coordinate of paddle
*/
void drawPaddle(int y)
{
  // Draw the verticle back wall part
  gfx->fillRect(0, y, PaddleWidth, PaddleHeight, WHITE);
}

/*!
  @brief  Updates and draws the paddle according to input from Joystick
  @param  Joystick_input      Latest Input from Joystick
*/
void updatePaddle(Joystick_input Joystick_input)
{
  int Old_Paddle_y_pos = Paddle_y_pos;

  // update paddle position according to input
  Paddle_y_pos = Paddle_y_pos + (Joystick_input.y / 133);
  if (Paddle_y_pos < 0)
  {
    Paddle_y_pos = 0;
  }
  else if (Paddle_y_pos > (screenHeight - PaddleHeight))
  {
    Paddle_y_pos = screenHeight - PaddleHeight;
  }

  // Erase only the part of the old paddle that is not overlapped by the new paddle
  if (Paddle_y_pos > Old_Paddle_y_pos)
  {
    gfx->fillRect(0, Old_Paddle_y_pos, PaddleWidth, Paddle_y_pos - Old_Paddle_y_pos, BLACK);
  }
  else if (Paddle_y_pos < Old_Paddle_y_pos)
  {
    gfx->fillRect(0, Paddle_y_pos + PaddleHeight, PaddleWidth, Old_Paddle_y_pos - Paddle_y_pos, BLACK);
  }

  // Draw new paddle
  drawPaddle(Paddle_y_pos);
}
/*!
  @brief  Draws the ball of Ping Game.
  @param  x      Center-point x coordinate
  @param  y      Center-point y coordinate
*/
void drawBall(int x, int y)
{
  gfx->fillCircle(x, y, BallRadius, WHITE);
}

/*!
  @brief  Updates the ball according to physics
*/
void updateBall()
{
  // Erase Ball
  gfx->fillCircle(ball.x, ball.y, BallRadius, BLACK);

  // Update the position
  ball.x += ball.dx;
  ball.y += ball.dy;

  // Draw ball
  drawBall(ball.x, ball.y);
}
/*!
  @brief  Updates Ping according to FSM states and guards
  @param  curState Current state of Game
  @param  Joystick_input Latest input from Joystick
  @return New Ping State
*/
PingState PingUpdateFSM(PingState curState, Joystick_input Joystick_input)
{
  PingState nextState;
  switch (curState)
  {
  case sStart_Game:
    gfx->fillScreen(BLACK);
    drawBackWall();
    drawPaddle(Paddle_y_pos);
    drawBall(120, 160);
    nextState = sMove_Step;
    break;

  case sMove_Step:
    updatePaddle(Joystick_input);
    updateBall();
    // if (checkCollision())
    // {
    //   nextState = sCollision_Step;
    // }

    break;

  case sCollision_Step:
    // if (x < x_paddle)
    // {
    //   nextState = sGAME_OVER;
    // }
    // else
    // {
    //   nextState = sMove_Step;
    // }
    // break;
  case sGAME_OVER:
    // DisplayGameOver
    break;
  }
  return nextState;
}
