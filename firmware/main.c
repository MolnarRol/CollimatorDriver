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
#include <ECOM_interface.h>
#include <MDA_interface.h>
#include <PWM_interface.h>
#include <SCI.h>

boolean output_en = False_b;
U16 data_aU16[16] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
/**
 * @brief Main function
 */
void main(void)
{
    /* Initialization */
    mcu_vInitClocks();                                          /* Initialize uC clock system. */
    ATB_Init();
    MDA_Init();
    PWM_Init();
    SCI_Init();
    /* Main loop */
    while(1)
    {
        PWM_SetOutputEnable(output_en);
        ECOM_MainHandler();
//        AC_MainHandler();                                       /* Application control main handler. */
//        ECOM_TxRequest(data_aU16, 12);
    }
}

/**
 * @brief Error handler
 */
#pragma RETAIN ( ErrorHandler )
void ErrorHandler(void)
{
    DINT;                                                       /* Disable interrupts. */
    for(;;)                                                     /* Endless loop. */
    {
        // Do nothing.
    }
}
