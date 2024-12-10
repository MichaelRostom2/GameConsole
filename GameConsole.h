#pragma once

#include <Arduino.h>
#include <Arduino_GFX_Library.h>
#include "Ping/Ping_utils.h"
#include "Utils/UART/uart_utils.h"
#include "Utils/Watchdog/watchdog_utils.h"
#include "Utils/ISR/isr_utils.h"
#include "Dodge/Dodge_utils.h"

#define JOYSTICK_VRX_PIN A0 // Arduino pin connected to Joystick VRX pin
#define JOYSTICK_VRY_PIN A1 // Arduino pin connected to Joystick VRY pin

/* Holds both x and y values for joystick input */
struct Joystick_input
{
  int x; // Joystick input in x-direction (-501,522). Starting position is 0.
  int y; // Joystick input in y-direction (-504,519). Starting position is 0.
};

extern const int BUTTON_IGNORE_TIME;
extern int button_ignore_timer;

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

/* Functions */
Joystick_input pollInputs();
void updateFSM(Joystick_input joystickInput, float deltaTime);
void EraseRect(float OldX, float OldY, float x, float y, int size);
void drawMenu();