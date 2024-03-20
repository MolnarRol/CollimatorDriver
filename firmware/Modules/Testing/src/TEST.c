/*
 * TEST.c
 *
 *  Created on: Mar 20, 2024
 *      Author: roland
 */
#include <TEST.h>
#include <TRAN.h>
#include <ATB_interface.h>
#include <PWM_interface.h>

const U32 scalar_update_period__us__U32 = (U32)200;
F32 scalar_freq__Hz__F32 = (F32)0.0;;
U16 test = 0;

volatile TRAN_struct tran_s =
{
    .dq_s =
    {
         .d_F32 = (F32)0.0,
         .q_F32 = (F32)1.0
    },
    .angle__rad__F32 = (F32)0.0
};

void TEST_ScalarMotorMovementHandler(void)
{
    tran_s.angle__rad__F32 += (F32)6.2832 * scalar_freq__Hz__F32 * (F32)50.0e-6;
    if( tran_s.angle__rad__F32 > (F32)6.2832 )
    {
        tran_s.angle__rad__F32 -= (F32)6.2832;
    }
    TRAN_DqToAbc(&tran_s);

    PWM_SetCompareValues(PWM_DUTY_TO_CMP_dMU16( (tran_s.abc_s.a_F32 + (F32)1.0) / (F32)2.0),
                         PWM_DUTY_TO_CMP_dMU16( (tran_s.abc_s.b_F32 + (F32)1.0) / (F32)2.0),
                         PWM_DUTY_TO_CMP_dMU16( (tran_s.abc_s.c_F32 + (F32)1.0) / (F32)2.0));

    DELAY_US(50);
}



