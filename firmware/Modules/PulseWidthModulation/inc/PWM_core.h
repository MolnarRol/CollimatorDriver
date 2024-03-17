/*
 * PWM_core.h
 *
 *  Created on: Mar 13, 2024
 *      Author: roland
 */

#ifndef MODULES_PULSEWIDTHMODULATION_INC_PWM_CORE_H_
#define MODULES_PULSEWIDTHMODULATION_INC_PWM_CORE_H_

#include <main.h>
#include <PWM_interface.h>

#define PWM_DEFAULT_CMP_dU16        ( (U16)0xFFFF )
static void PWM_InitGpioPins(void);

#endif /* MODULES_PULSEWIDTHMODULATION_INC_PWM_CORE_H_ */
