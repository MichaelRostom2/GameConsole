#include "isr_utils.h"

const unsigned int D2_PORT = 1;
const unsigned int D2_PIN = 4;
const unsigned int D2_IRQ = 1;

const unsigned int D3_PORT = 1;
const unsigned int D3_PIN = 5;
const unsigned int D3_IRQ = 0;

unsigned int CPU_INT_1;
unsigned int CPU_INT_2;

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

void switchToPing()
{
    currentGame = PING;
    PING_CURRENT_STATE = Ping_Start_Game;
}

void switchToDodge()
{
    currentGame = DODGE;
    DODGE_CURRENT_STATE = Dodge_Start_Game;
}

void resetISR()
{
    Serial.println("resetISR triggered");
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

void homeISR()
{
    currentGame = MENU;
    Serial.println("homeISR triggered");

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
    drawMenu();
    // FSM stuff here
}

void buttonsISRSetup()
{

    // Setup Interrupts for home and reset buttons
    // NVIC_DisableIRQ((IRQn_Type)CPU_INT_1);
    // NVIC_DisableIRQ((IRQn_Type)CPU_INT_2);

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
