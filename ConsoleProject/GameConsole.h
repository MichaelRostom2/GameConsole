#pragma once
#include <Arduino_GFX_Library.h>
/* MOSI: 11 */
/* SCK: 13 */

enum gameMode {
  MENU,
  PING,
  DODGE
};

enum gameMode currentGame;

// Hardware setup
Arduino_DataBus *bus = new Arduino_HWSPI(8 /* DC */, 10 /* CS */);
Arduino_GFX *gfx = new Arduino_ILI9341(bus, 9 /* RST */);

// Constants
const int screenWidth = 240;
const int screenHeight = 320;

/* Home Functions */

/* Dodge Functions */
void dodge_setup();
void dodge_loop();

/* Ping Functions */
void ping_setup();
void ping_loop();

/* Watchdog Functions */