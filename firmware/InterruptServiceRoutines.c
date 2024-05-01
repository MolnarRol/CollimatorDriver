/*
 * InterruptServiceRoutines.c
 *
 *  Created on: Mar 28, 2024
 *      Author: roland
 */
#include <main.h>
#include <ATB_interface.h>
#include <TEST.h>
#include "PWM_interface.h"
#include "FOC.h"
#include "InterruptServiceRoutines.h"
#include "MDA_interface.h"
#include "dispCtrl.h"

U16 display_counter_U16;
char buffer[12] = {};
boolean enable_FOC = 0;
boolean alarm_state = 0;

extern F32 refPos_F32;
extern F32 max_speed_F32;
extern F32 max_accel_F32;

inline void ISR_MotorControlHandler(void)
{
    TEST_PIN_SET_dM;

    ATB_IncrementTime();
    /* Motor control functionality. Called every PWM cycle. */
    if(enable_FOC){
        kukam_prud();
        FOC_CalculateOutput(refPos_F32, max_speed_F32, max_accel_F32);
    }

    display_counter_U16++;
    if(display_counter_U16 > 5000)
    {
        display_counter_U16 = 0;
        dispCtrl_vSetPosition(1,3);
        float_to_char_array(MDA_GetData_ps()->angular_position__rad__F32, &buffer, 2);
        dispCtrl_u16PutString(&buffer);
        dispCtrl_u16PutString(" mm");
    }
//    TEST_PIN_RESET_dM;
//    TEST_ScalarMotorMovementHandler();

//    TEST_SteppingHandler();

}


