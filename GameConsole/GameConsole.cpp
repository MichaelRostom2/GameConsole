#include "GameConsole.h"

/* Instantiate screen gfx object*/
/* MOSI: 11 */
/* SCK: 13 */
Arduino_DataBus *bus = new Arduino_HWSPI(8 /* DC */, 10 /* CS */);
Arduino_GFX *gfx = new Arduino_ILI9341(bus, 9 /* RST */);

/* Screen parameter */
const int screenWidth = 240;
const int screenHeight = 320;

/* Start in MENU */
gameMode currentGame = MENU;

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
        }
        else if (joystickInput.x < -250)
        {

            currentGame = DODGE;
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

void setup()
{
    Serial.begin(9600);

    /* Setup Screen */
    gfx->begin();
    gfx->setRotation(0);

    // TODO: initialise watchdog
    // watchdogSetup();

    randomSeed(analogRead(0));

    /* Draw Menu on screen */
    drawMenu();
}
void loop()
{
    Joystick_input joystickInput = pollInputs();
    // Serial.println("x = ");
    // Serial.println(joystickInput.x);
    // Serial.println("y = ");
    // Serial.println(joystickInput.y);
    static float deltaTime = 0.01;
    updateFSM(joystickInput, deltaTime);
    delay(10);
}