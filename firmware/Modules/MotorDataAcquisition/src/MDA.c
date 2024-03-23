/*
 * MDA.c
 *
 *  Created on: Mar 13, 2024
 *      Author: roland
 */
#include <MDA_core.h>
#include <MDA_interface.h>

void MDA_Init(void)
{
    MDA_AdcInit();
}

static inline void MDA_AdcInit(void)
{
    EALLOW;
    /* Enable ADC clocks. */
    CpuSysRegs.PCLKCR13.bit.ADC_A = 1;
    CpuSysRegs.PCLKCR13.bit.ADC_B = 1;

    /* Enable power to ADCs. */
    AdcaRegs.ADCCTL1.bit.ADCPWDNZ = 1;
    AdcbRegs.ADCCTL1.bit.ADCPWDNZ = 1;

    AdcaRegs.ADCCTL1.bit.INTPULSEPOS = 1;
    AdcbRegs.ADCCTL1.bit.INTPULSEPOS = 1;

    AdcaRegs.ADCINTFLG.bit.ADCINT1;
//    AdcaResultRegs.ADC

    EDIS;
}

