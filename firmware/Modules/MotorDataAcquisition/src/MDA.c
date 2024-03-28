/*
 * MDA.c
 *
 *  Created on: Mar 13, 2024
 *      Author: roland
 */
#include <MDA_core.h>
#include <MDA_interface.h>
#include <InterruptServiceRoutines.h>

MDA_Data_struct s_MDA_data_s;

void MDA_Init(void)
{
    MDA_AdcInit();
}

interrupt void MDA_AdcConverstionCompleteIsr(void)
{
    EINT;
    MDA_UpdateData();
    ISR_MotorControlHandler();
    AdcaRegs.ADCINTFLGCLR.bit.ADCINT1           = (U16)1;                   /* Clear interrupt flag. */
    PieCtrlRegs.PIEACK.bit.ACK1                 = (U16)1;
}

static inline void MDA_AdcInit(void)
{
    EALLOW;
    /* Enable ADC clocks. */
    CpuSysRegs.PCLKCR13.bit.ADC_A               = (U16)1;
    CpuSysRegs.PCLKCR13.bit.ADC_B               = (U16)1;

    /* Prescaling clocks -> ADC_CLK = CPU_CLK / 2 = 100 MHz. */
    AdcaRegs.ADCCTL2.bit.PRESCALE               = (U16)2;
    AdcbRegs.ADCCTL2.bit.PRESCALE               = (U16)2;

    AdcaRegs.ADCCTL1.bit.INTPULSEPOS            = (U16)1;

    /* Enable power to ADCs. */
    AdcaRegs.ADCCTL1.bit.ADCPWDNZ               = (U16)1;
    AdcbRegs.ADCCTL1.bit.ADCPWDNZ               = (U16)1;

    DELAY_US(MDA_ADC_STARTUP_DELAY__us__U32);                               /* Wait for ADC power up. */

    /* Channel ADCINA1 - Phase U current measurement. */
    AdcaRegs.ADCSOC1CTL.bit.CHSEL               = (U16)1;                   /* ADCIN1 */
    AdcaRegs.ADCSOC1CTL.bit.TRIGSEL             = (U16)0x09;                /* ADCTRIG9 - ePWM3, ADCSOCA */
    AdcaRegs.ADCSOC1CTL.bit.ACQPS               = (U16)19;                  /* Sample current 20 ADC clocks. */

    /* Channel ADCINB0 - Phase V current measurement. */
    AdcbRegs.ADCSOC0CTL.bit.CHSEL               = (U16)0;                   /* ADCIN0 */
    AdcbRegs.ADCSOC0CTL.bit.TRIGSEL             = (U16)0x09;                /* ADCTRIG9 - ePWM3, ADCSOCA */
    AdcbRegs.ADCSOC0CTL.bit.ACQPS               = (U16)19;                  /* Sample current 20 ADC clocks. */

    /* Channel ADCINA0 - Phase W current measurement. */
    AdcaRegs.ADCSOC0CTL.bit.CHSEL               = (U16)0;                   /* ADCIN0 */
    AdcaRegs.ADCSOC0CTL.bit.TRIGSEL             = (U16)0x09;                /* ADCTRIG9 - ePWM3, ADCSOCA */
    AdcaRegs.ADCSOC0CTL.bit.ACQPS               = (U16)19;                  /* Sample current 20 ADC clocks. */

    /* Channel ADCINB1 - DC link voltage measurement. */
    AdcbRegs.ADCSOC1CTL.bit.CHSEL               = (U16)1;                   /* ADCIN1 */
    AdcbRegs.ADCSOC1CTL.bit.TRIGSEL             = (U16)0x0a;                /* ADCTRIG10 - ePWM3, ADCSOCB */
    AdcbRegs.ADCSOC1CTL.bit.ACQPS               = (U16)19;                  /* Sample current 20 ADC clocks. */

    /* Interrupt setup. */
    AdcaRegs.ADCINTSEL1N2.bit.INT1E             = (U16)1;                   /* Enable interrupt 1. */
    AdcaRegs.ADCINTSEL1N2.bit.INT1SEL           = (U16)1;                   /* Setting end of conversion 1 to interrupt source. */
    AdcaRegs.ADCINTFLGCLR.bit.ADCINT1           = (U16)1;                   /* Clear ADC INT 1 flag. */

    DINT;
    PieCtrlRegs.PIECTRL.bit.ENPIE               = (U16)1;                   /* Enable interrupt vector table peripheral. */
    PieCtrlRegs.PIEIER1.bit.INTx1               = (U16)1;

    IER                                         |= M_INT1;                  /* Enable CPU interrupt line. */
    PieVectTable.ADCA1_INT                      = &MDA_AdcConverstionCompleteIsr;
    EINT;

    EDIS;
}

static inline void MDA_UpdateData(void)
{
    const F32 phase_U_current__A__F32 = MDA_PHASE_CURRENT_FROM_ADC_VAL_dMF32(MDA_ADC_U_CURRENT_CONV_RES_d);
    const F32 phase_V_current__A__F32 = MDA_PHASE_CURRENT_FROM_ADC_VAL_dMF32(MDA_ADC_V_CURRENT_CONV_RES_d);
    const F32 phase_W_current__A__F32 = MDA_PHASE_CURRENT_FROM_ADC_VAL_dMF32(MDA_ADC_W_CURRENT_CONV_RES_d);
    s_MDA_data_s.dc_link_voltage__V__F32 = MDA_DC_LINK_VOLTAGE_FROM_ADC_VAL_dMF32(MDA_ADC_DC_LINK_CONV_RES_d);

}

const MDA_Data_struct* MDA_GetData_ps(void)
{
    return (const MDA_Data_struct*)&s_MDA_data_s;
}
