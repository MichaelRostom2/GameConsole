#pragma once
#include <Arduino_GFX_Library.h>
#include "Arduino.h"
#include "watchdog.h"
#include "Ping_utils.h"
/* MOSI: 11 */
/* SCK: 13 */

enum gameMode
{
  MENU,
  PING,
  DODGE
};

extern gameMode currentGame;

// Hardware setup
extern Arduino_DataBus *bus;
extern Arduino_GFX *gfx;

// Constants
extern const int screenWidth;
extern const int screenHeight;

/* Home Functions */

/* Dodge Functions */
void dodge_setup();
void dodge_loop();

/* Ping Functions */
void ping_setup();
void ping_loop();

/* Watchdog Functions */
