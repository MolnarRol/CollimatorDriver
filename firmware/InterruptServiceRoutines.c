/*
 * InterruptServiceRoutines.c
 *
 *  Created on: Mar 28, 2024
 *      Author: roland
 */
#include <main.h>
#include <ATB_interface.h>
#include <MTCL_interface.h>

boolean alarm_state = 0;

inline void ISR_MotorControlHandler(void)
{
    ATB_IncrementTime();
    /* Motor control functionality. Called every PWM cycle. */
    MTCL_MainHandler();
}


