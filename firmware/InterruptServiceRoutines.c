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


boolean enable_FOC = 0;

inline void ISR_MotorControlHandler(void)
{



    ATB_IncrementTime();
    /* Motor control functionality. Called every PWM cycle. */
    if(enable_FOC){
        kukam_prud();
        FOC_CalculateOutput(0);

    }

    //TEST_ScalarMotorMovementHandler();

//    TEST_SteppingHandler();

}


