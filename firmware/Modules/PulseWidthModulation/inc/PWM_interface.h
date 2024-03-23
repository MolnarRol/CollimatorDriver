/*
 * PWM_interface.h
 *
 *  Created on: Mar 13, 2024
 *      Author: roland
 */
#include <app_types.h>

#ifndef MODULES_PULSEWIDTHMODULATION_INC_PWM_INTERFACE_H_
#define MODULES_PULSEWIDTHMODULATION_INC_PWM_INTERFACE_H_

#define PWM_FREQ__Hz__dF32                          ( (F32)20000.0 )
#define PWM_PERIOD_dU16                             ( (U16)2499 )

#define PWM_DUTY_TO_CMP_dMU16(duty_F32)             ( PWM_PERIOD_dU16 - (U16)((duty_F32) * (F32)PWM_PERIOD_dU16) )
#define PWM_DUTY_PERC_TO_CMP_dMU16(duty__perc__F32) ( PWM_PERIOD_dU16 - (U16)((duty__perc__F32 / (F32)100.0) * (F32)PWM_PERIOD_dU16) )

void PWM_Init(void);
void PWM_SetCompareValues(const U16 comp_U_U16, const U16 comp_V_U16, const U16 comp_W_U16);
void PWM_SetOutputEnable(const boolean enable_b);

#endif /* MODULES_PULSEWIDTHMODULATION_INC_PWM_INTERFACE_H_ */
