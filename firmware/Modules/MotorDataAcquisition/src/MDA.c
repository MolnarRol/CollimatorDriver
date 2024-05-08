/**
 * @file MDA.c
 * @brief Motor data acquisition module
 * @details Module for readeading and evaluating measurement data for further regulation and control.
 *
 * =================================================================
 * @author Bc. Roland Molnar
 *
 * =================================================================
 * KEM, FEI, TUKE
 * @date 30.03.2024
 * @defgroup MDA Motor data acquisition
 * @{
 */
#include <math.h>
#include <MDA_core.h>
#include <MDA_interface.h>
#include <InterruptServiceRoutines.h>
#include <Modules/Miscellaneous/inc/TRAN.h>
#include <FAST_MATH_FUNC.h>
#include <TEST.h>
#include <LW_filter.h>

/* static */ MDA_Data_struct s_MDA_data_s;                                        /**< Module data structure. */

/* Phase current offset variables. */
static boolean s_MDA_phase_currents_calibrated_b       = False_b;
const static F32 s_MDA_phase_u_offset_current__A__dF32  = (F32)0.0;
const static F32 s_MDA_phase_v_offset_current__A__dF32  = (F32)0.0;
const static F32 s_MDA_phase_w_offset_current__A__dF32  = (F32)0.0;

/*********************************************************************************************************************************************
 *                                                      Static functions                                                                     *
 *********************************************************************************************************************************************/

/**
 * @brief ADC conversion completion interrupt.
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
 * @brief ADC initialization function.
 */
static inline void MDA_AdcInit(void)
{
    EALLOW;
    /* Enable ADC clocks. */
    CpuSysRegs.PCLKCR13.bit.ADC_A       = (U16)1;                                   /* Enable ADCA clock. */
    CpuSysRegs.PCLKCR13.bit.ADC_B       = (U16)1;                                   /* Enable ADCB clock. */

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

    DINT;                                                                           /* Globally disable interrupts. */
    PieCtrlRegs.PIECTRL.bit.ENPIE       = (U16)1;                                   /* Enable interrupt vector table peripheral. */
    PieCtrlRegs.PIEIER1.bit.INTx1       = (U16)1;
    IER                                 |= M_INT1;                                  /* Enable CPU interrupt line. */
    PieVectTable.ADCA1_INT              = &MDA_AdcConverstionCompleteIsr;
    EINT;                                                                           /* GLobally enable interrupts. */

    EDIS;
}

/**
 * @brief Quadrature encoder interface initialization
 */
static inline void MDA_QepInit(void)
{
    EALLOW;
    CpuSysRegs.PCLKCR4.bit.EQEP1        = (U16)1;                                   /* Enable clocks for eQEP1. */
    GpioCtrlRegs.GPAMUX2.bit.GPIO20     = (U16)1;                                   /* Quadrature encoder A track. */
    GpioCtrlRegs.GPAGMUX2.bit.GPIO20    = (U16)0;

    GpioCtrlRegs.GPAMUX2.bit.GPIO21     = (U16)1;                                   /* Quadrature encoder B track. */
    GpioCtrlRegs.GPAGMUX2.bit.GPIO21    = (U16)0;

    if(MDA_ENC_SWAP_DIR_db == True_b)
    {
        EQep1Regs.QDECCTL.bit.SWAP      = (U16)1;                                   /* Swapping A and B tracks. */
    }

    EQep1Regs.QEPCTL.bit.FREE_SOFT      = (U16)3;                                   /* Run even when the CPU is stopped. */
    EQep1Regs.QEPCTL.bit.PCRM           = (U16)1;                                   /* Reset on maximum */
    EQep1Regs.QPOSMAX                   = MDA_ENC_CPR_dU16 - (U16)1;                /* Setting maximum to counts per revolution of rotary encoder. */

    /* Delta time measurement setup. */
    EQep1Regs.QEPCTL.bit.QCLM           = (U16)1;                                   /* Latch on unit time out. */
    EQep1Regs.QUPRD                     = (U32)( MDA_ENC_DELTA_PULSE_SAMPLE_TIME__s__dF32
                                                  * (F32) 200.0E6) - (U32)1;
    EQep1Regs.QCAPCTL.bit.CCPS          = (U16)1;                                   /* eQEP capture timer clock prescaler /4. */

    /* eQep pulse Watchdog setup. */
    EQep1Regs.QWDPRD                    = (U16)( ((F32)200.0 / (F32)64.0) * (F32)MDA_ENC_NO_PULSE_TIMEOUT__us__dU16 );

    /* Enable eQep internal timers. */
    EQep1Regs.QEPCTL.bit.QPEN           = (U16)1;                                   /* Enable encoder interface counter. */
    EQep1Regs.QCAPCTL.bit.CEN           = (U16)1;
    EQep1Regs.QEPCTL.bit.UTE            = (U16)1;
    EQep1Regs.QEPCTL.bit.WDE            = (U16)1;                                   /* QEP watchdog enable. */
    EDIS;
}

/**
 * @brief Routine for calibrating phase current sensors DC offset
 */
void MDA_CalibratePhaseCurrentsOffsets(void)
{
    if(s_MDA_phase_currents_calibrated_b == True_b)
    {
        s_MDA_phase_currents_calibrated_b = True_b;
        return;
    }
    DINT;                                                                           /* Disable interrupts. */
    U32 adc_read_sum_u_U32 = (U32)0;
    U32 adc_read_sum_v_U32 = (U32)0;
    U32 adc_read_sum_w_U32 = (U32)0;
    U16 step_index_U16;

    for(step_index_U16 = (U16)0; step_index_U16 < MDA_PHASE_CURRENT_CALIBRATION_STEPS_U16; step_index_U16++)
    {
        AdcbRegs.ADCSOCFRC1.all |= (U16)0x0003;                                     /* Force SOC0 and SOC1 for ADC B. */
        AdcaRegs.ADCSOCFRC1.all |= (U16)0x0003;                                     /* Force SOC0 and SOC1 for ADC A. */

        while(AdcaRegs.ADCINTFLG.bit.ADCINT1 != (U16)1) __asm__("NOP");             /* Wait for conversion. */
        AdcaRegs.ADCINTFLGCLR.bit.ADCINT1 = (U16)1;                                 /* Clear interrupt flag. */

        adc_read_sum_u_U32 += (U32)MDA_ADC_U_CURRENT_CONV_RES_d;                    /* Add result to phase u sum. */
        adc_read_sum_v_U32 += (U32)MDA_ADC_V_CURRENT_CONV_RES_d;                    /* Add result to phase v sum. */
        adc_read_sum_w_U32 += (U32)MDA_ADC_W_CURRENT_CONV_RES_d;                    /* Add result to phase w sum. */

        DELAY_US( MDA_PHASE_CURRENT_CALIBRATION_STEP_DELAY__us__dU16 );             /* Wait. */
    }

    /* Averaging ADC readout for each phase and offset calculation. */
    *(F32*)&s_MDA_phase_u_offset_current__A__dF32 = MDA_PHASE_CURRENT_FROM_ADC_VAL_dMF32(adc_read_sum_u_U32 / (U32)MDA_PHASE_CURRENT_CALIBRATION_STEPS_U16);
    *(F32*)&s_MDA_phase_v_offset_current__A__dF32 = MDA_PHASE_CURRENT_FROM_ADC_VAL_dMF32(adc_read_sum_v_U32 / (U32)MDA_PHASE_CURRENT_CALIBRATION_STEPS_U16);
    *(F32*)&s_MDA_phase_w_offset_current__A__dF32 = MDA_PHASE_CURRENT_FROM_ADC_VAL_dMF32(adc_read_sum_w_U32 / (U32)MDA_PHASE_CURRENT_CALIBRATION_STEPS_U16);

    EINT;                                                                           /* Enable interrupts. */
}

#pragma FUNC_ALWAYS_INLINE ( MDA_UpdateData )
/**
 * @brief Update data structure with newly acquired measurement data.
 */
static inline void MDA_UpdateData(void)
{
    TRAN_struct current_transf_s = {0};
    /*Counter for speed calculation each 250e-6 = 5 interrupts*/
    static U16 Speed_calc_intrr_counter = 0;
    Speed_calc_intrr_counter++;

    /* Speed and motor position. */
    s_MDA_data_s.rotor_mech_angle__rad__F32 =  TWO_PI_dF32 * ((F32)MDA_GetRawRotorMechAngle_U16() / (F32)U16_MAX);
//    s_MDA_data_s.rotor_el_angle__rad__F32 = (U16)( ((U32)s_MDA_data_s.rotor_mech_angle__rad__F32 * (U32)MOTOR_POLE_PAIRS_dU16) % (U32)U16_MAX );
    s_MDA_data_s.rotor_el_angle__rad__F32 = FM_RemainderAfterFloatDivision_F32(s_MDA_data_s.rotor_mech_angle__rad__F32 * (F32)MOTOR_POLE_PAIRS_dU16, TWO_PI_dF32);
    current_transf_s.angle__rad__F32 = s_MDA_data_s.rotor_el_angle__rad__F32;

    if(Speed_calc_intrr_counter == 5){
        s_MDA_data_s.rotor_mech_speed__rad_s1__F32 = LW_Filter_Speed_CalculateOutput(MDA_get_mech_speed_rads1_F32(MDA_delta_pos__pulses__S32()));
        s_MDA_data_s.rotor_el_speed__rad_s1__F32 = s_MDA_data_s.rotor_mech_speed__rad_s1__F32 * (F32)MOTOR_POLE_PAIRS_dU16;
        Speed_calc_intrr_counter = 0;
    }

    /*Angular position calculation*/
    s_MDA_data_s.angular_position__rad__F32 = (F32)( ( (F32)s_MDA_data_s.linear_position_enc_counter_U32 / (F32)MDA_ENC_CPR_dU16 ) * TWO_PI_dF32 );

    /* Linear position calculation. */
    s_MDA_data_s.linear_position__mm__F32 = ((F32)s_MDA_data_s.linear_position_enc_counter_U32 / (F32)MDA_ENC_CPR_dU16) * MOTOR_LINEAR_TRANN_TRANSFER__rev_mm1__dF32;

    /* Write calculated phase current value from ADC conversions. */

    /* From left -> right
     * L1: Black
     * L2: Red
     * L3: Yellow
     * */
    MDA_GetRawPhaseCurrents( &current_transf_s.abc_s.a_F32,
                             &current_transf_s.abc_s.b_F32,
                             &current_transf_s.abc_s.c_F32 );

    /* Current transformation UVW -> DQ */
    TRAN_AbcToDq(&current_transf_s);

    /* Write transformed current results to data structure/ */
    s_MDA_data_s.currents_s.id__A__F32 = LW_Filter_id_CalculateOutput(current_transf_s.dq_s.d_F32);
    s_MDA_data_s.currents_s.iq__A__F32 = LW_Filter_iq_CalculateOutput(current_transf_s.dq_s.q_F32);

    /* Write calculate DC link voltage to data structure. */
    s_MDA_data_s.dc_link_voltage__V__F32 = MDA_DC_LINK_VOLTAGE_FROM_ADC_VAL_dMF32(MDA_ADC_DC_LINK_CONV_RES_d);
}

/**
 * @brief Calculation of unsigned 16 bit integer delta value
 * @param current_count_U16 is a current QEP count
 * @param prev_count_U16 is previous QEP count
 * @returns delta value.
 */
static inline U16 MDA_EncoderGetPulseDelta_U16(const U16 prev_count_U16, const U16 current_count_U16)
{

        U16 delta_U16 = 0;
        if(EQep1Regs.QEPSTS.bit.QDF == (U16)1)
        {
            if(current_count_U16 >= prev_count_U16) delta_U16 = current_count_U16 - prev_count_U16;
            else                                    delta_U16 = EQep1Regs.QPOSMAX - prev_count_U16 + current_count_U16;
        }
        else
        {
            if(current_count_U16 <= prev_count_U16) delta_U16 = prev_count_U16 - current_count_U16;
            else                                    delta_U16 = EQep1Regs.QPOSMAX - current_count_U16 + prev_count_U16;
        }
    return delta_U16;
}

#pragma FUNC_ALWAYS_INLINE ( MDA_GetRawRotorMechAngle_U16 )
/**
 * @brief Get current motor shaft angle from position of origin.
 * @returns Mechanical angle in Unsigned 16-bit integer form. 0xFFFF/2 = 180 degrees
 */
static inline U16 MDA_GetRawRotorMechAngle_U16(void)
{
    static U16 s_prev_pos_U16 = (U16)0;
    const U16 current_pos_U16 = EQep1Regs.QPOSCNT;

    const U16 pos_delta_U16 = MDA_EncoderGetPulseDelta_U16(s_prev_pos_U16, current_pos_U16);
    s_prev_pos_U16 = current_pos_U16;

    if(EQep1Regs.QEPSTS.bit.QDF == (U16)1)  s_MDA_data_s.linear_position_enc_counter_U32 += pos_delta_U16;
    else                                    s_MDA_data_s.linear_position_enc_counter_U32 -= pos_delta_U16;

    return (U16)( ((U32)current_pos_U16 * (U32)U16_MAX) / (U32)(MDA_ENC_CPR_dU16 - (U16)1) );
}

S32 MDA_delta_pos__pulses__S32(){
    static  S32 MDA_prev_pos__pulses__S32 = 0;   //predch. pozicia
    S32 MDA_current_pos__pulses__S32 = EQep1Regs.QPOSCNT; //aktualna pos.

    S32  MDA_delta_pos__pulses__ = MDA_current_pos__pulses__S32 - MDA_prev_pos__pulses__S32;

    if(EQep1Regs.QEPSTS.bit.QDF == 1 && MDA_delta_pos__pulses__ < 0){
        MDA_delta_pos__pulses__ += EQep1Regs.QPOSMAX;
    }
    if(EQep1Regs.QEPSTS.bit.QDF == 0 && MDA_delta_pos__pulses__ > 0){
            MDA_delta_pos__pulses__ -= EQep1Regs.QPOSMAX;
    }


    MDA_prev_pos__pulses__S32 = MDA_current_pos__pulses__S32;
    return MDA_delta_pos__pulses__;
}

/**
 * @brief Calculate mechanical speed from QEP pulse delta.
 * @param delta_pos__pulses__S32 is a QEP counter delta.
 * @retval Mechanical speed in rad/s.
 */
F32 MDA_get_mech_speed_rads1_F32(S32 delta_pos__pulses__S32){
    F32 delta_angle__rad__F32 = (F32)delta_pos__pulses__S32*TWO_PI_dF32/(F32)(MDA_ENC_CPR_dU16);

    return delta_angle__rad__F32/SPEED_SAMPLE_TIMEdF32;
}

/**
 * @brief Writes phase currents value to parameter adresses
 * @param u_pF32 is a pointer to phase U current value.
 * @param v_pF32 is a pointer to phase V current value.
 * @param w_pF32 is a pointer to phase W current value.
 */
void MDA_GetRawPhaseCurrents(F32 * const u_pF32, F32 * const v_pF32, F32 * const w_pF32)
{
    *u_pF32 = MDA_PHASE_CURRENT_FROM_ADC_VAL_dMF32(MDA_ADC_U_CURRENT_CONV_RES_d) - s_MDA_phase_u_offset_current__A__dF32;
    *v_pF32 = MDA_PHASE_CURRENT_FROM_ADC_VAL_dMF32(MDA_ADC_V_CURRENT_CONV_RES_d) - s_MDA_phase_v_offset_current__A__dF32;
    *w_pF32 = MDA_PHASE_CURRENT_FROM_ADC_VAL_dMF32(MDA_ADC_W_CURRENT_CONV_RES_d) - s_MDA_phase_w_offset_current__A__dF32;
}

/*********************************************************************************************************************************************
 *                                                      Interface functions                                                                  *
 *********************************************************************************************************************************************/

/**
 * @brief Motor data acquisition initialization function.
 */
void MDA_Init(void)
{
    MDA_AdcInit();
    MDA_QepInit();
}

/**
 * @brief Access motor data for motor control.
 * @returns pointer to acquired motor currents, speed and position data.
 */
inline const MDA_Data_struct* MDA_GetData_ps(void)
{
    return (const MDA_Data_struct*)&s_MDA_data_s;
}

/**
 * @brief Resets linear position
 * @deprecated
 */
inline void MDA_ResetLinearPosition(void)
{
    s_MDA_data_s.linear_position_enc_counter_U32 = (U32)0;
    s_MDA_data_s.linear_position__mm__F32 = (F32)0.0;
}

/**
 * @}
 */
