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
#include <CircularBuffer.h>

#include <ECOM_interface.h>

CB_Handle_struct x;

/**
 * @brief Main function
 */
void main(void)
{
    /* Initialization */
    mcu_vInitClocks();                                          /* Initialize uC clock system. */
    ATB_Init();
    SCI_Init();

//    CB_Init(&x);
    /* Main loop */
    while(1)
    {
        AC_MainHandler();                                       /* Application control main handler. */
        ECOM_MainHandler();
    }
}
