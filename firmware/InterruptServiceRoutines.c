/*
 * InterruptServiceRoutines.c
 *
 *  Created on: Mar 28, 2024
 *      Author: roland
 */
#include <main.h>
#include <ATB_interface.h>
#include <TEST.h>
#include <TRAN.h>
#include "PWM_interface.h"
//#include "FOC.h"
#include "InterruptServiceRoutines.h"




inline void ISR_MotorControlHandler(void)
{

    ATB_IncrementTime();
    /* Motor control functionality. Called every PWM cycle. */

//    TEST_ScalarMotorMovementHandler();

//    TEST_SteppingHandler();

}


