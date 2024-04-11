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


boolean output_en = False_b;
/**
 * @brief Main function
 */

float BufferA[1000];
float BufferB[1000];
float BufferC[1000];

Uint16 u16buffer_counter;

void main(void)
{
    /* Initialization */
    mcu_vInitClocks();                                          /* Initialize uC clock system. */
    ATB_Init();
    MDA_Init();
    PWM_Init();
    SCI_Init();

    PWM_SetOutputEnable(True_b);

    CommutationAlignment();

    PWM_SetOutputEnable(True_b);

    /* Main loop */
    while(1)
    {

        ECOM_MainHandler();
//        AC_MainHandler();                                       /* Application control main handler. */

        /* TEST */

        if(u16buffer_counter < 1000){

            BufferA[u16buffer_counter] = tran_s.angle__rad__F32;
            BufferB[u16buffer_counter] = 6.2831 * ( MDA_GetData_ps()->rotor_el_angle__rad__F32 / (F32)U16_MAX );
            BufferC[u16buffer_counter] = tran_s.abc_s.a_F32;
            u16buffer_counter++;

        }
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
