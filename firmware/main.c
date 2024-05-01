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
#include <PI_Controller.h>
#include "spi.h"
#include "dispCtrl.h"

extern TRAN_struct tran_s;
extern boolean enable_FOC;
extern boolean alarm_state;

boolean output_en = True_b;
/**
 * @brief Main function
 */

//float BufferA[1000];
//float BufferB[1000];
//float BufferC[1000];

Uint16 u16buffer_counter;
Uint16 FOC_counter;

void main(void)
{
    /* Initialization */

    mcu_vInitClocks();                                          /* Initialize uC clock system. */
    ATB_Init();
    PWM_Init();
    SCI_Init();
    spi_vInit(9600);
    dispCtrl_vInitDisplay();
    DELAY_US(50000);
    CommutationAlignment();
    MDA_Init();
    TEST_PinInit();
    MDA_CalibratePhaseCurrentsOffsets();

    dispCtrl_vSetPosition(1,1);
    dispCtrl_u16PutString("Collimator Blade");
    dispCtrl_vSetPosition(1,2);
    dispCtrl_u16PutString("Position: ");

    /*Redudant reset of PI controller structures*/

    PI_ctrl_Init(&PI_id_current_controller);
    PI_ctrl_Init(&PI_iq_current_controller);
    PI_ctrl_Init(&PI_speed_controller);
    PI_ctrl_Init(&PI_position_controller);

    enable_FOC = 1;

    /* Main loop */
    while(1)
    {
        PWM_SetOutputEnable(output_en);
        ECOM_MainHandler();
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
