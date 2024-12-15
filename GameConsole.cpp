#include "GameConsole.h"

// Joystick offsets so that starting position gives x=0, y=0. (manual adjust accordingly)
const int JOYSTICK_X_OFFSET = 504;
const int JOYSTICK_Y_OFFSET = 509;

/* Instantiate screen gfx object*/
/* MOSI: 11 */
/* SCK: 13 */
Arduino_DataBus *bus = new Arduino_HWSPI(8 /* DC */, 10 /* CS */);
Arduino_GFX *gfx = new Arduino_ILI9341(bus, 9 /* RST */);

/* Screen parameter */
const int screenWidth = 240;
const int screenHeight = 320;
int button_ignore_timer = 0;
const int BUTTON_IGNORE_TIME = 3;

/* Start in MENU */
gameMode currentGame = MENU;

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
  @brief  Updates Game according to FSM states and guards
  @param  Joystick_input Latest input from Joystick
*/
void updateFSM(Joystick_input joystickInput, float deltaTime)
{
  switch (currentGame)
  {
  case MENU:

    if (joystickInput.x > 250)
    {

      currentGame = PING;
      PING_CURRENT_STATE = PING_START;
    }
    else if (joystickInput.x < -250)
    {

      currentGame = DODGE;
      DODGE_CURRENT_STATE = DODGE_START;
    }

    break;

  case PING:
    playPing(joystickInput, deltaTime);
    break;

  case DODGE:
    playDodge(joystickInput);
    break;
  }
}
/*!
  @brief  Draws Menu screen
*/
void drawMenu()
{
  gfx->fillScreen(BLACK);
  gfx->setCursor((screenWidth / 2) - 55, 0);
  gfx->setTextSize(5);
  gfx->setTextColor(WHITE);
  gfx->println("Menu");
  gfx->setTextSize(3);
  gfx->setCursor(0, screenHeight / 2);
  gfx->println("Dodge");
  gfx->setCursor((screenWidth / 2) + 45, screenHeight / 2);
  gfx->println("Ping");
}
/*!
  @brief  Erases the difference between two rectangle.
  @param  OldX Top left x position of the old rectangle
  @param  OldY Top left y position of the old rectangle
  @param  x Top left x position of the new rectangle
  @param  y Top left y position of the new rectangle
  @param  size Size of the rectangle
*/
void EraseRect(float oldX, float oldY, float x, float y, int size)
{
  float xDiff = x - oldX;
  float yDiff = y - oldY;

  if (x < 0 || y < 0)
  {
    // Prevent drawing outside of screen
    gfx->fillRect(oldX, oldY, size, size, BLACK);
  }
  else
  {
    // Erase difference on x-axis
    if (xDiff > 0)
    {
      gfx->fillRect(oldX, oldY, xDiff + 1, size, BLACK);
    }
    else if (xDiff < 0)
    {
      gfx->fillRect(x + size, oldY, -xDiff + 1, size, BLACK);
    }

    // Erase difference on y-axis
    if (yDiff > 0)
    {
      gfx->fillRect(oldX, oldY, size, yDiff + 1, BLACK);
    }
    else if (yDiff < 0)
    {
      gfx->fillRect(oldX, y + size, size, -yDiff + 1, BLACK);
    }
  }
}

void setup()
{
  Serial.begin(9600);

  /* Setup Screen */
  gfx->begin();
  gfx->setRotation(0);

  buttonsISRSetup();
  //  initWDT();

  randomSeed(analogRead(0));

  /* Draw Menu on screen */
  drawMenu();
}
void loop()
{
  // int enterLoopTime = millis();
  Joystick_input joystickInput = pollInputs();
  // Serial.println("x = ");
  // Serial.println(joystickInput.x);
  // Serial.println("y = ");
  // Serial.println(joystickInput.y);
  static float deltaTime = 0.01;
  if (button_ignore_timer > 0)
  {
    button_ignore_timer -= 1;
  }
  // petWDT();
  updateFSM(joystickInput, deltaTime);
  delay(10);
  // int exitLoopTime = millis();
  // deltaTime = exitLoopTime - enterLoopTime;
  // Serial.println(deltaTime);
}
