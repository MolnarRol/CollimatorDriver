/**
 * @file main.c
 * @brief Main application file
 * @details Details
 *
 * =================================================================
 * @author Bc. Samuel Fertal
 * @author Bc. Vadym Holysh
 * @author Bc. Roland Molnar
 *
 * =================================================================
 * KEM, FEI, TUKE
 * @date 27.02.2024
 */
#include <main.h>
#include <ATB_interface.h>
#include <AC_interface.h>
#include <SCI.h>
#include <ECOM_interface.h>
#include <DEBUG.h>

U32 led_blink_time_U32;
/**
 * @brief Main function
 */
void main(void)
{
    /* Initialization */
    mcu_vInitClocks();                                          /* Initialize uC clock system. */
    ATB_Init();
    SCI_Init();
    DEBUG_Init();

    led_blink_time_U32 = ATB_GetTicks_U32();

    /* Main loop */
    while(1)
    {
        ECOM_MainHandler();
        AC_MainHandler();                                       /* Application control main handler. */
        if( ATB_CheckTicksPassed_U16(led_blink_time_U32, ATB_MS_TO_TICKS_dM_U32(100)) )
        {
            led_blink_time_U32 = ATB_GetTicks_U32();
            GpioDataRegs.GPBTOGGLE.bit.GPIO34 = 1;
//            GpioDataRegs.GPATOGGLE.bit.GPIO31 = 1;
        }
    }
}
