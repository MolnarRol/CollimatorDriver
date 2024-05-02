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
//U32 counter = 0;
//char buffer[12] = {};

void main(void)
{
    /* Initialization */

    mcu_vInitClocks();                                          /* Initialize uC clock system. */
    spi_vInit(900000);
    dispCtrl_vInitDisplay();
    ATB_Init();
    PWM_Init();
    SCI_Init();
    CommutationAlignment();
    MDA_Init();
    TEST_PinInit();
    MDA_CalibratePhaseCurrentsOffsets();

    //dispCtrl_clear();
    dispCtrl_vSetPosition(1,1);
    DELAY_US(100);
    dispCtrl_u16PutString("Collimator Blade");
    DELAY_US(100);
    dispCtrl_vSetPosition(1,2);
    DELAY_US(100);
    dispCtrl_u16PutString("Position: ");
    DELAY_US(100);
    dispCtrl_vSetPosition(1,4);
    DELAY_US(100);
    dispCtrl_u16PutString("<-5 mm    +5 mm>");
    DELAY_US(100);

    /*Redundant reset of PI controller structures*/

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
        //DisplayRefresh();
//        counter++;
//        if(counter > 100000){
//        counter = 0;
//        dispCtrl_vSetPosition(1,3);
//        float_to_char_array(MDA_GetData_ps()->angular_position__rad__F32, &buffer, 2);
//        dispCtrl_u16PutString(&buffer);
//        dispCtrl_u16PutString(" mm  ");
//        }
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
