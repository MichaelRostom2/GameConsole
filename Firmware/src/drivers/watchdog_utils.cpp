#include "watchdog_utils.h"

const unsigned int WDT_INT = getNextCPUINT(15);
/* [NOT USED] ISR when WDT triggers*/
void wdtISR()
{
    Serial.println("WOOF!!!");
    while (true)
        ;
}
/*!
    @brief  Initializes the watchdog timer
*/
void initWDT()
{
    /*
     Watchdog triggers every 2.796 seconds.
     Using CSK bits 1000, and TOPS 10, the watchdog will trigger every 67108864 PCLKB cycles which run at 24 Mhz.
     67108864 /(24 Mhz) = 2.796 seconds
     */
    R_WDT->WDTCR = ((0b11 << R_WDT_WDTCR_RPSS_Pos) | (0b11 << R_WDT_WDTCR_RPES_Pos) | (0b1000 << R_WDT_WDTCR_CKS_Pos) | (0b10 << R_WDT_WDTCR_TOPS_Pos));

    R_DEBUG->DBGSTOPCR_b.DBGSTOP_WDT = 0;
    R_WDT->WDTSR = 0;

    R_WDT->WDTRCR = 1 << R_WDT_WDTRCR_RSTIRQS_Pos; // Restarts the console
    R_ICU->IELSR[WDT_INT] = 0x025;
    // CMSIS
    NVIC_SetVector((IRQn_Type)WDT_INT, (uint32_t)&wdtISR);
    NVIC_SetPriority((IRQn_Type)WDT_INT, 13);
    NVIC_EnableIRQ((IRQn_Type)WDT_INT);
}
/*!
    @brief  Pets the watchdog timer
*/
void petWDT()
{
    R_WDT->WDTRR = 0x00;
    R_WDT->WDTRR = 0xff;
}
