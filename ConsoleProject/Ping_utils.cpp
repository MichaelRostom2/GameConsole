#include "Ping_utils.h"

// Game drawing variables
const int BackWallThickness = 8;
const int PaddleWidth = 8;
const int PaddleHeight = 64;
const int BallRadius = 8;
const int Gravity = 5;

// Structures
struct Ball
{
  int x, y;
  int dx, dy;
  int bounciness;
  bool gravity_right;
};
Ball ball = Ball{120, 160, 1, 1, 0, false};
int Paddle_y_pos = (screenHeight / 2) - (PaddleHeight / 2);

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
}

/*!
  @brief  Erases the paddle of Ping Game.
  @param  gfx    Pointer to Arduino_GFX screen object
  @param  y      Top-Left y coordinate of paddle
*/
void erasePaddle(Arduino_GFX *gfx, int y)
{
  // Draw the verticle back wall part
  gfx->fillRect(0, y, PaddleWidth, PaddleHeight, BLACK);
  // TODO: remove flickering by only redrawing the parts that need to be redrawn
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

void movePaddle(Joystick_input Joystick_input)
{
  Paddle_y_pos = Paddle_y_pos + (Joystick_input.y / 133);
  if (Paddle_y_pos < 0)
  {
    Paddle_y_pos = 0;
  }
  else if (Paddle_y_pos > (screenHeight - PaddleHeight))
  {
    Paddle_y_pos = screenHeight - PaddleHeight;
  }
}
void eraseBall()
{
  gfx->fillCircle(ball.x, ball.y, BallRadius, BLACK);
}
void updateBall()
{
  eraseBall();

  // Update the position
  ball.x += ball.dx;
  ball.y += ball.dy;
  drawBall(gfx, ball.x, ball.y);
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
  Serial.println(curState);
  switch (curState)
  {
  case sStart_Game:
    gfx->fillScreen(BLACK);
    drawBackWall(gfx, 240, 320);
    drawPaddle(gfx, Paddle_y_pos);
    drawBall(gfx, 120, 160);
    nextState = sMove_Step;
    break;

  case sMove_Step:
    erasePaddle(gfx, Paddle_y_pos);
    movePaddle(Joystick_input);
    drawPaddle(gfx, Paddle_y_pos);
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
PingState PING_CURRENT_STATE = sStart_Game;

void playPing(Joystick_input joystickInput)
{
  PING_CURRENT_STATE = PingUpdateFSM(PING_CURRENT_STATE, joystickInput);
}
