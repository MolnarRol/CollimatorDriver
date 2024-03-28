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
#include <math.h>

F32 scalar_freq__Hz__F32 = (F32)250.0;

volatile TRAN_struct tran_s =
{
    .dq_s =
    {
         .d_F32 = (F32)0.0,
         .q_F32 = (F32)2.0
    },
    .angle__rad__F32 = (F32)0.0
};

void TEST_ScalarMotorMovementHandler(void)
{
//    tran_s.angle__rad__F32 += (F32)6.2832 * scalar_freq__Hz__F32 * (F32)50.0e-6;
    tran_s.angle__rad__F32 += (F32)2.0 * M_PI * scalar_freq__Hz__F32 * (F32)50.0e-6;

    if( tran_s.angle__rad__F32 > (F32)2.0 * M_PI )
    {
        tran_s.angle__rad__F32 -= (F32)2.0 * M_PI;
    }
    else if( tran_s.angle__rad__F32 < (F32)0.0 )
    {
        tran_s.angle__rad__F32 += (F32)2.0 * M_PI;
    }

    TRAN_DqToAbc(&tran_s);

    PWM_SetCompareValues(PWM_DUTY_TO_CMP_dMU16( (tran_s.abc_s.a_F32 / (F32)24.0) + (F32)0.5 ),
                         PWM_DUTY_TO_CMP_dMU16( (tran_s.abc_s.b_F32 / (F32)24.0) + (F32)0.5 ),
                         PWM_DUTY_TO_CMP_dMU16( (tran_s.abc_s.c_F32 / (F32)24.0) + (F32)0.5 ));
}



