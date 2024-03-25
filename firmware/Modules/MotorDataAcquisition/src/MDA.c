/*
 * MDA.c
 *
 *  Created on: Mar 13, 2024
 *      Author: roland
 */
#include <MDA_core.h>
#include <MDA_interface.h>

static MDA_Data_struct MDA_data_s;

void MDA_Init(void)
{
    MDA_AdcInit();
}

static inline void MDA_AdcInit(void)
{
    EALLOW;
    /* Enable ADC clocks. */
    CpuSysRegs.PCLKCR13.bit.ADC_A               = (U16)1;
    CpuSysRegs.PCLKCR13.bit.ADC_B               = (U16)1;

    /* Enable power to ADCs. */
    AdcaRegs.ADCCTL1.bit.ADCPWDNZ               = (U16)1;
    AdcbRegs.ADCCTL1.bit.ADCPWDNZ               = (U16)1;

    AdcaRegs.ADCCTL1.bit.INTPULSEPOS            = (U16)1;
    AdcbRegs.ADCCTL1.bit.INTPULSEPOS            = (U16)1;

//    AdcaRegs.ADCINTFLG.bit.ADCINT1;
//    AdcaResultRegs.ADC
    AdcaRegs.ADCSOC0CTL.bit.CHSEL               = (U16)0;
    AdcaRegs.ADCSOC0CTL.bit.TRIGSEL             = (U16)0x9;
    AdcaRegs.ADCSOC0CTL.bit.ACQPS               = (U16)0x8F;


    EDIS;
}

const MDA_Data_struct* MDA_GetData_ps(void)
{
    return (const MDA_Data_struct*)&MDA_data_s;
}
