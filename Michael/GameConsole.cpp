#include "Ping_utils.h"
Arduino_DataBus *bus = new Arduino_HWSPI(8 /* DC */, 10 /* CS */);
Arduino_GFX *gfx = new Arduino_ILI9341(bus, 9 /* RST */);
void setup()
{
    Serial.begin(9600);
    while (!Serial)
        ;
    // TODO: Initialise System
}

void loop()
{
    /* FIXME: This is only for ping */
    Joystick_input joystickInput = pollInputs();
    // Serial.print(joystickInput.x);
    // Serial.print("_");
    // Serial.println(joystickInput.y);
    static PingState CURRENT_STATE = sStart_Game;
    CURRENT_STATE = PingUpdateFSM(gfx, CURRENT_STATE, millis(), joystickInput);
    delay(10);
}