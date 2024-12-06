#include "GameConsole.h"

const unsigned int D2_PORT = 1;
const unsigned int D2_PIN = 4;
const unsigned int D2_IRQ = 1;

const unsigned int D3_PORT = 1;
const unsigned int D3_PIN = 5;
const unsigned int D3_IRQ = 0;

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

const unsigned int CPU_INT_1 = getNextCPUINT(1);
const unsigned int CPU_INT_2 = getNextCPUINT(CPU_INT_1);

void watchdogSetup()
{

    // Setup Interrupts for home and reset buttons
    NVIC_DisableIRQ((IRQn_Type)CPU_INT_1);
    NVIC_DisableIRQ((IRQn_Type)CPU_INT_2);

    R_PFS->PORT[D2_PORT].PIN[D2_PIN].PmnPFS = R_PFS_PORT_PIN_PmnPFS_ISEL_Msk;
    R_PFS->PORT[D3_PORT].PIN[D3_PIN].PmnPFS = R_PFS_PORT_PIN_PmnPFS_ISEL_Msk;
    R_ICU->IRQCR[D2_IRQ] = 0x1UL;
    R_ICU->IRQCR[D3_IRQ] = 0x1UL;
    R_ICU->IELSR[CPU_INT_1] = 0x002;
    R_ICU->IELSR[CPU_INT_2] = 0x001;

    NVIC_SetVector((IRQn_Type)CPU_INT_1, (uint32_t)&resetISR);
    NVIC_SetPriority((IRQn_Type)CPU_INT_1, 14);
    NVIC_EnableIRQ((IRQn_Type)CPU_INT_1);
    NVIC_SetVector((IRQn_Type)CPU_INT_2, (uint32_t)&homeISR);
    NVIC_SetPriority((IRQn_Type)CPU_INT_2, 14);
    NVIC_EnableIRQ((IRQn_Type)CPU_INT_2);
}

void switchToPing()
{

    ping_setup();
}

void switchToDodge()
{
    dodge_setup();
}

void resetISR()
{
    switch (currentGame)
    {
    case MENU:
        R_ICU->IELSR_b[CPU_INT_1].IR = 0;
        NVIC_ClearPendingIRQ((IRQn_Type)CPU_INT_1);
        break;
    case PING:
        R_ICU->IELSR_b[CPU_INT_1].IR = 0;
        NVIC_ClearPendingIRQ((IRQn_Type)CPU_INT_1);
        switchToPing();
        break;
    case DODGE:
        R_ICU->IELSR_b[CPU_INT_1].IR = 0;
        NVIC_ClearPendingIRQ((IRQn_Type)CPU_INT_1);
        switchToDodge();
        break;
    }
}

void homeISR()
{
    currentGame = MENU;

    R_ICU->IELSR_b[CPU_INT_2].IR = 0;
    NVIC_ClearPendingIRQ((IRQn_Type)CPU_INT_2);

    // FSM stuff here
}
