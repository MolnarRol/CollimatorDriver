/*
 * TEST.h
 *
 *  Created on: Mar 20, 2024
 *      Author: roland
 */

#ifndef MODULES_TESTING_INC_TEST_H_
#define MODULES_TESTING_INC_TEST_H_

#include <mcu.h>
#include <app_types.h>

#define TEST_PIN_SET_dM         ( GpioDataRegs.GPCSET.bit.GPIO72 = (U16)1 )
#define TEST_PIN_RESET_dM       ( GpioDataRegs.GPCCLEAR.bit.GPIO72 = (U16)1 )
#define TEST_PIN_TOGGLE_dM      ( GpioDataRegs.GPCTOGGLE.bit.GPIO72 = (U16)1 )

void TEST_ScalarMotorMovementHandler(void);
void TEST_PinInit(void);
void kukam_prud();

#endif /* MODULES_TESTING_INC_TEST_H_ */
