#include "Ping_utils.h"

// Game drawing variables
const int BackWallThickness = 8;
const int PaddleWidth = 8;
const int PaddleHeight = 64;
const int BallSize = 16;
float Gravity = 40;
float bounciness = 0.1;

// Ping tracking variables
Ball ball = Ball{120, 160, 0, 0, bounciness};
int Paddle_y_pos = (screenHeight / 2) - (PaddleHeight / 2);

PingState PING_CURRENT_STATE = sStart_Game;

/*!
  @brief  Plays the Ping Game by calling updateFSM of Ping
  @param  joystickInput Latest input from Joystick
*/
void playPing(Joystick_input joystickInput, float deltaTime)
{
  PING_CURRENT_STATE = PingUpdateFSM(PING_CURRENT_STATE, joystickInput);
}

/*!
  @brief  Draws the back wall of Ping Game.
*/
void drawBackWall()
{
  //  Draw the verticle back wall part
  gfx->fillRect(screenWidth - BackWallThickness, 0, BackWallThickness, screenHeight, RED);
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
  gfx->fillRect(x, y, BallSize, BallSize, GREENYELLOW);
}

void eraseBall()
{
  // Erase Ball
  gfx->fillRect(ball.x, ball.y, BallSize, BallSize, BLACK);
}

/*!
  @brief  Updates the ball according to physics
*/
void updateBall()
{

  eraseBall();
  Gravity += 35 * 0.1;
  // Update the velocity
  ball.dx += Gravity * 0.01; // FIXME use actual deltaTime
  // Update the position
  ball.x += ball.dx * 0.01;
  ball.y += ball.dy * 0.01;

  // Resolve any collisions
  CollideBall();

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

    if (checkGameOver())
    {
      nextState = sGAME_OVER;
    }
    else
    {
      nextState = sMove_Step;
    }
    break;
  case sGAME_OVER:
    displayGameOver();
    break;
  }
  return nextState;
}
/*!
  @brief  Displays Game Over Screen
*/
void displayGameOver()
{
  // gfx->fillScreen(BLACK);
  gfx->setTextColor(WHITE);
  gfx->setTextSize(4);
  gfx->setCursor(screenWidth / 15, screenHeight / 3);
  gfx->println("Game Over");
}

void horizontalBounce()
{
  ball.dx *= -ball.bounciness;
  Gravity *= -1;
}

void CollideBall()
{
  if (ball.x <= PaddleWidth)
  {
    if (ball.y >= Paddle_y_pos - BallSize && ball.y <= Paddle_y_pos + PaddleHeight)
    {
      horizontalBounce();
      ball.x = PaddleWidth + 1;
      // Adjust dy depending on paddle hit location
      float centerYDiff = (ball.y + BallSize / 2) - (Paddle_y_pos + PaddleHeight / 2);
      ball.dy += centerYDiff * 2;
    }
  }
  else if (ball.x >= screenWidth - BackWallThickness - BallSize)
  {
    horizontalBounce();
    ball.x = screenWidth - BackWallThickness - BallSize - 1;
    float rand = (random(0, 1024) * (80 / 1024)) - 40;
    ball.dy += rand;
  }
  else if (ball.y <= 0)
  {
    ball.dy = -ball.dy;
    ball.y = 0;
  }
  else if (ball.y >= screenHeight - BallSize)
  {
    ball.dy = -ball.dy;
    ball.y = screenHeight - BallSize;
  }
}
bool checkGameOver()
{
  if (ball.x <= 0)
  {
    return true;
  }
  return false;
}