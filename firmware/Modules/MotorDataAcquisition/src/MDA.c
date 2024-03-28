/*
 * MDA.c
 *
 *  Created on: Mar 13, 2024
 *      Author: roland
 */
#include <MDA_core.h>
#include <MDA_interface.h>
#include <TRAN.h>
#include <InterruptServiceRoutines.h>

MDA_Data_struct s_MDA_data_s;                                        /**<  */

/**
 * @brief
 */
void MDA_Init(void)
{
    MDA_AdcInit();
    MDA_QepInit();
}

/**
 * @brief
 */
interrupt void MDA_AdcConverstionCompleteIsr(void)
{
    EINT;
    MDA_UpdateData();
    ISR_MotorControlHandler();
    AdcaRegs.ADCINTFLGCLR.bit.ADCINT1   = (U16)1;                                   /* Clear interrupt flag. */
    PieCtrlRegs.PIEACK.bit.ACK1         = (U16)1;
}

/**
 * @brief
 */
static inline void MDA_AdcInit(void)
{
    EALLOW;
    /* Enable ADC clocks. */
    CpuSysRegs.PCLKCR13.bit.ADC_A       = (U16)1;
    CpuSysRegs.PCLKCR13.bit.ADC_B       = (U16)1;

    /* Prescaling clocks -> ADC_CLK = CPU_CLK / 2 = 100 MHz. */
    AdcaRegs.ADCCTL2.bit.PRESCALE       = (U16)2;
    AdcbRegs.ADCCTL2.bit.PRESCALE       = (U16)2;

    AdcaRegs.ADCCTL1.bit.INTPULSEPOS    = (U16)1;

    /* Enable power to ADCs. */
    AdcaRegs.ADCCTL1.bit.ADCPWDNZ       = (U16)1;
    AdcbRegs.ADCCTL1.bit.ADCPWDNZ       = (U16)1;

    DELAY_US(MDA_ADC_STARTUP_DELAY__us__U32);                                       /* Wait for ADC power up. */

    /* Channel ADCINA1 - Phase U current measurement. */
    AdcaRegs.ADCSOC1CTL.bit.CHSEL       = (U16)1;                                   /* ADCIN1 */
    AdcaRegs.ADCSOC1CTL.bit.TRIGSEL     = (U16)0x09;                                /* ADCTRIG9 - ePWM3, ADCSOCA */
    AdcaRegs.ADCSOC1CTL.bit.ACQPS       = MDA_ADC_CURRENT_MEASUREMENT_WINDOWS_dU16; /* Sample current 20 ADC clocks. */

    /* Channel ADCINB0 - Phase V current measurement. */
    AdcbRegs.ADCSOC0CTL.bit.CHSEL       = (U16)0;                                   /* ADCIN0 */
    AdcbRegs.ADCSOC0CTL.bit.TRIGSEL     = (U16)0x09;                                /* ADCTRIG9 - ePWM3, ADCSOCA */
    AdcbRegs.ADCSOC0CTL.bit.ACQPS       = MDA_ADC_CURRENT_MEASUREMENT_WINDOWS_dU16; /* Sample current 20 ADC clocks. */

    /* Channel ADCINA0 - Phase W current measurement. */
    AdcaRegs.ADCSOC0CTL.bit.CHSEL       = (U16)0;                                   /* ADCIN0 */
    AdcaRegs.ADCSOC0CTL.bit.TRIGSEL     = (U16)0x09;                                /* ADCTRIG9 - ePWM3, ADCSOCA */
    AdcaRegs.ADCSOC0CTL.bit.ACQPS       = MDA_ADC_CURRENT_MEASUREMENT_WINDOWS_dU16; /* Sample current 20 ADC clocks. */

    /* Channel ADCINB1 - DC link voltage measurement. */
    AdcbRegs.ADCSOC1CTL.bit.CHSEL       = (U16)1;                                   /* ADCIN1 */
    AdcbRegs.ADCSOC1CTL.bit.TRIGSEL     = (U16)0x0a;                                /* ADCTRIG10 - ePWM3, ADCSOCB */
    AdcbRegs.ADCSOC1CTL.bit.ACQPS       = MDA_ADC_CURRENT_MEASUREMENT_WINDOWS_dU16; /* Sample current 20 ADC clocks. */

    /* Interrupt setup. */
    AdcaRegs.ADCINTSEL1N2.bit.INT1E     = (U16)1;                                   /* Enable interrupt 1. */
    AdcaRegs.ADCINTSEL1N2.bit.INT1SEL   = (U16)1;                                   /* Setting end of conversion 1 to interrupt source. */
    AdcaRegs.ADCINTFLGCLR.bit.ADCINT1   = (U16)1;                                   /* Clear ADC INT 1 flag. */
    DINT;
    PieCtrlRegs.PIECTRL.bit.ENPIE       = (U16)1;                                   /* Enable interrupt vector table peripheral. */
    PieCtrlRegs.PIEIER1.bit.INTx1       = (U16)1;
    IER                                 |= M_INT1;                                  /* Enable CPU interrupt line. */
    PieVectTable.ADCA1_INT              = &MDA_AdcConverstionCompleteIsr;
    EINT;
    EDIS;
}

/**
 * @brief
 */
static inline void MDA_QepInit(void)
{
    EALLOW;
    CpuSysRegs.PCLKCR4.bit.EQEP1        = (U16)1;       /* Enable clocks for eQEP1. */
    GpioCtrlRegs.GPAMUX2.bit.GPIO20     = (U16)1;       /* Quadrature encoder A track. */
    GpioCtrlRegs.GPAGMUX2.bit.GPIO20    = (U16)0;

    GpioCtrlRegs.GPAMUX2.bit.GPIO21     = (U16)1;       /* Quadrature encoder B track. */
    GpioCtrlRegs.GPAGMUX2.bit.GPIO21    = (U16)0;


    if(MDA_END_SWAP_DIR_db == True_b)
    {
        EQep1Regs.QDECCTL.bit.SWAP      = (U16)1;
    }

    EQep1Regs.QEPCTL.bit.FREE_SOFT      = (U16)3;
    EQep1Regs.QEPCTL.bit.PCRM           = (U16)1;       /* Reset on maximum */
    EQep1Regs.QPOSMAX                   = MDA_ENC_CPR_dU16 - (U16)1;
    EQep1Regs.QEPCTL.bit.QPEN = 1;
    EDIS;
}

/**
 * @brief Update data structure with newly acquired measurement data.
 */
static inline void MDA_UpdateData(void)
{
    TRAN_struct current_transf_s = {0};

    /* Speed and motor position. */

    /* Write calculated phase current value from ADC conversions. */
    current_transf_s.abc_s.a_F32 = MDA_PHASE_CURRENT_FROM_ADC_VAL_dMF32(MDA_ADC_U_CURRENT_CONV_RES_d);
    current_transf_s.abc_s.b_F32 = MDA_PHASE_CURRENT_FROM_ADC_VAL_dMF32(MDA_ADC_V_CURRENT_CONV_RES_d);
    current_transf_s.abc_s.c_F32 = MDA_PHASE_CURRENT_FROM_ADC_VAL_dMF32(MDA_ADC_W_CURRENT_CONV_RES_d);

    /* Current transformation UVW -> DQ */
    TRAN_AbcToDq(&current_transf_s);

    /* Write transformed current results to data structure/ */
    s_MDA_data_s.currents_s.id__A__F32 = current_transf_s.dq_s.d_F32;
    s_MDA_data_s.currents_s.iq__A__F32 = current_transf_s.dq_s.q_F32;

    /* Write calculate DC link voltage to data structure. */
    s_MDA_data_s.dc_link_voltage__V__F32 = MDA_DC_LINK_VOLTAGE_FROM_ADC_VAL_dMF32(MDA_ADC_DC_LINK_CONV_RES_d);
}


/**
 * @brief Access motor data for motor control.
 * @returns pointer to acquired motor currents, speed and position data.
 */
const MDA_Data_struct* MDA_GetData_ps(void)
{
    return (const MDA_Data_struct*)&s_MDA_data_s;
}
