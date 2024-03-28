/*
 * InterruptServiceRoutines.c
 *
 *  Created on: Mar 28, 2024
 *      Author: roland
 */
#include <main.h>
#include <TEST.h>

inline void ISR_MotorControlHandler(void)
{
    /* Motor control functionality. Called every PWM cycle. */
    TEST_ScalarMotorMovementHandler();
}

