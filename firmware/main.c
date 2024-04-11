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
#include "InterruptServiceRoutines.h"
#include "FOC.h"
#include "TEST.h"

extern TRAN_struct tran_s;
extern boolean enable_FOC;


boolean output_en = True_b;
/**
 * @brief Main function
 */

//float BufferA[1000];
//float BufferB[1000];
//float BufferC[1000];

Uint16 u16buffer_counter;



void main(void)
{
    /* Initialization */
    mcu_vInitClocks();                                          /* Initialize uC clock system. */
    ATB_Init();
    MDA_Init();
    PWM_Init();
    SCI_Init();

    TEST_PinInit();

    CommutationAlignment();                             // premenuj potom na FOC_com alingemnt


    //PWM_SetOutputEnable(output_en);

    enable_FOC = 1;

    /* Main loop */
    while(1)
    {

        PWM_SetOutputEnable(output_en);
//        ECOM_MainHandler();
//        AC_MainHandler();
        /* Application control main handler. */

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
