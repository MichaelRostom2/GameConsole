#include <Arduino.h>
#include <Arduino_GFX_Library.h>

#define JOYSTICK_VRX_PIN A0 // Arduino pin connected to Joystick VRX pin
#define JOYSTICK_VRY_PIN A1 // Arduino pin connected to Joystick VRY pin

/* Holds both x and y values for joystick input */
struct Joystick_input
{
    int x; // Joystick input in x-direction (-501,522). Starting position is 0.
    int y; // Joystick input in y-direction (-504,519). Starting position is 0.
};

Joystick_input pollInputs();

typedef enum
{
    sStartup_Step = 1,
    sDisplay_Menu = 2,
} ConsoleState;

ConsoleState ConsoleUpdateFSM();
