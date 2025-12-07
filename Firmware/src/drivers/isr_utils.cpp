#include "isr_utils.h"

const unsigned int D2_PORT = 1;
const unsigned int D2_PIN = 4;
const unsigned int D2_IRQ = 1;

const unsigned int D3_PORT = 1;
const unsigned int D3_PIN = 5;
const unsigned int D3_IRQ = 0;

unsigned int CPU_INT_1;
unsigned int CPU_INT_2;

/*!
    @brief  Get the next unused CPU interrupt
    @param  start The CPU interrupt to start searching from
    @return The next unused CPU interrupt
*/
unsigned int getNextCPUINT(unsigned int start)
{
    /* Get next unused CPU interrupt > start */
    unsigned int tryInt = start + 1;
    while (tryInt < 32)
    {
        if (NVIC_GetEnableIRQ((IRQn_Type)tryInt) == 0)
        {
            return tryInt;
        }
        tryInt++;
    }
}
/*!
    @brief  Switches the current game to Ping
*/
void switchToPing()
{
    currentGame = PING;
    PING_CURRENT_STATE = PING_START;
}

/*!
    @brief  Switches the current game to Dodge
*/
void switchToDodge()
{
    currentGame = DODGE;
    DODGE_CURRENT_STATE = DODGE_START;
}
/*!
    @brief  Resets the game state.
*/
void resetISR()
{
    Serial.println("resetISR triggered");
#ifndef UNIT_TEST
    R_ICU->IELSR_b[CPU_INT_1].IR = 0;
    NVIC_ClearPendingIRQ((IRQn_Type)CPU_INT_1);
    if (button_ignore_timer > 0)
    {
        return;
    }
    else
    {
        button_ignore_timer = BUTTON_IGNORE_TIME;
    }
#endif
    switch (currentGame)
    {
    case MENU:
        break;
    case PING:
        switchToPing();
        break;
    case DODGE:
        switchToDodge();
        break;
    }
}
/*!
    @brief  Switches the current game to the menu
*/
void homeISR()
{
    currentGame = MENU;
    Serial.println("homeISR triggered");
#ifndef UNIT_TEST
    R_ICU->IELSR_b[CPU_INT_2].IR = 0;
    NVIC_ClearPendingIRQ((IRQn_Type)CPU_INT_2);
    if (button_ignore_timer > 0)
    {
        return;
    }
    else
    {
        button_ignore_timer = BUTTON_IGNORE_TIME;
    }
#endif
    drawMenu();
}

/*!
    @brief  Sets up the interrupt service routines for the home and reset buttons
*/
void buttonsISRSetup()
{
    R_PFS->PORT[D2_PORT].PIN[D2_PIN].PmnPFS = R_PFS_PORT_PIN_PmnPFS_ISEL_Msk;
    R_PFS->PORT[D3_PORT].PIN[D3_PIN].PmnPFS = R_PFS_PORT_PIN_PmnPFS_ISEL_Msk;
    R_ICU->IRQCR[D2_IRQ] = 0x1UL;
    R_ICU->IRQCR[D3_IRQ] = 0x1UL;
    CPU_INT_1 = getNextCPUINT(1);
    CPU_INT_2 = getNextCPUINT(CPU_INT_1);
    R_ICU->IELSR[CPU_INT_1] = 0x002;
    R_ICU->IELSR[CPU_INT_2] = 0x001;

    NVIC_SetVector((IRQn_Type)CPU_INT_1, (uint32_t)&resetISR);
    NVIC_SetPriority((IRQn_Type)CPU_INT_1, 14);
    NVIC_EnableIRQ((IRQn_Type)CPU_INT_1);
    NVIC_SetVector((IRQn_Type)CPU_INT_2, (uint32_t)&homeISR);
    NVIC_SetPriority((IRQn_Type)CPU_INT_2, 14);
    NVIC_EnableIRQ((IRQn_Type)CPU_INT_2);
}
