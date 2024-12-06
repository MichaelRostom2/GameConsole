#include "GameConsole.h"
Arduino_DataBus *bus = new Arduino_HWSPI(8 /* DC */, 10 /* CS */);
Arduino_GFX *gfx = new Arduino_ILI9341(bus, 9 /* RST */);
const int screenWidth = 240;
const int screenHeight = 320;

gameMode currentGame = MENU;

void updateFSM(Joystick_input joystickInput)
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
        playPing(joystickInput);
        break;

    case DODGE:
        // playDodge(joystickInput);
        break;
    }
}
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
    delay(1);
}
void setup()
{
    Serial.begin(9600);
    currentGame = MENU;
    gfx->begin();
    gfx->setRotation(0);
    drawMenu();

    // initialise watchdog
    // watchdogSetup();
}
void loop()
{
    Joystick_input joystickInput = pollInputs();
    // Serial.println("x = ");
    // Serial.println(joystickInput.x);
    // Serial.println("y = ");
    // Serial.println(joystickInput.y);
    updateFSM(joystickInput);
    delay(10);
}
