#pragma once

#include "Arduino.h"
#include <Arduino_GFX_Library.h>
#include "Ping/Ping_utils.h"
#include "Utils/UART/uart_utils.h"
#include "Utils/Watchdog/watchdog_utils.h"
#include "Utils/ISR/isr_utils.h"
#include "Dodge/Dodge_utils.h"

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
static int pingHighScore = 0;
static int dodgeHighScore = 0;

/* Home Functions */
void updateFSM(Joystick_input joystickInput, float deltaTime);

/* Ping Functions */
void ping_setup();
void ping_loop();

/* Dodge Functions */
void dodge_setup();
void dodge_loop();

/* Watchdog Functions */

void Erase(float OldX, float OldY, float x, float y, int size);