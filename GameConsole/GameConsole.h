#pragma once

#include "Arduino.h"
#include <Arduino_GFX_Library.h>
#include "Ping_utils.h"
#include "watchdog.h"
#include "Dodge_utils.h"

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
void updateFSM(Joystick_input joystickInput, float deltaTime);

/* Ping Functions */
void ping_setup();
void ping_loop();

/* Dodge Functions */
void dodge_setup();
void dodge_loop();

/* Watchdog Functions */
