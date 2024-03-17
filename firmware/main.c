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

/**
 * @brief Main function
 */
void main(void)
{
    /* Initialization */
    mcu_vInitClocks();                                          /* Initialize uC clock system. */
    ATB_Init();
    PWM_Init();
    MDA_Init();
    SCI_Init();
    /* Main loop */
    while(1)
    {
        ECOM_MainHandler();
        AC_MainHandler();                                       /* Application control main handler. */
    }
}
