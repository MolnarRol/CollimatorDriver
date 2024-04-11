/*
 * TEST.c
 *
 *  Created on: Mar 20, 2024
 *      Author: roland
 */
#include <TEST.h>
#include <ATB_interface.h>
#include <PWM_interface.h>
#include <TRAN.h>
#include <FAST_MATH_FUNC.h>
#include <MDA_interface.h>
#include <MDA_core.h>
F32 scalar_freq__Hz__F32                = (F32)0.0;
F32 current_forced_el_angle__rad__F32   = (F32)0.0;

void TEST_ScalarMotorMovementHandler(void)
{
    current_forced_el_angle__rad__F32 += TWO_PI_dF32 * scalar_freq__Hz__F32 * (F32)50.0e-6;
    current_forced_el_angle__rad__F32 = FM_RemainderAfterFloatDivision_F32(current_forced_el_angle__rad__F32, TWO_PI_dF32);
    PWM_ForceAngle(current_forced_el_angle__rad__F32, (F32)1.25, (F32)24.0);
}



F32 current_a[512];
F32 current_b[512];
F32 current_c[512];


U16 index_prud=0;

void kukam_prud(){

    if(index_prud < 512){
        current_a[index_prud] = MDA_PHASE_CURRENT_FROM_ADC_VAL_dMF32(MDA_ADC_U_CURRENT_CONV_RES_d) + (F32)1.3;
        current_b[index_prud] = MDA_PHASE_CURRENT_FROM_ADC_VAL_dMF32(MDA_ADC_U_CURRENT_CONV_RES_d) - (F32)0.26;
        current_c[index_prud] = MDA_PHASE_CURRENT_FROM_ADC_VAL_dMF32(MDA_ADC_W_CURRENT_CONV_RES_d) - (F32)0.7;


        index_prud++;
    }

}

void TEST_SteppingHandler(void)
{
    static U32 last_exec_time_ticks_U32 = (U32)0;
    static F32 forced_angle_F32 = (F32)0;
    static boolean first_exec_b = True_b;

    if(first_exec_b)
    {
        last_exec_time_ticks_U32 = ATB_GetTicks_U32();
        first_exec_b = False_b;
        PWM_ForceAngle((F32)0.0, (F32)1.25, (F32)24.0);
    }

    if(ATB_CheckTicksPassed_U16(last_exec_time_ticks_U32, ATB_MS_TO_TICKS_dM_U32(250)))
    {
        last_exec_time_ticks_U32 = ATB_GetTicks_U32();
        PWM_ForceAngle(forced_angle_F32, (F32)1.25, (F32)24.0);
        forced_angle_F32 += TWO_PI_dF32 / (F32)4.0;
        forced_angle_F32 = FM_RemainderAfterFloatDivision_F32(forced_angle_F32, TWO_PI_dF32);


    }
}

void TEST_PinInit(void)
{
    EALLOW;
    GpioCtrlRegs.GPCDIR.bit.GPIO72 = (U16)1;
    EDIS;
}



