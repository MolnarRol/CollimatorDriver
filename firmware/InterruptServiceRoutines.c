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



//    TEST_PIN_RESET_dM;
//    TEST_ScalarMotorMovementHandler();

//    TEST_SteppingHandler();

}


