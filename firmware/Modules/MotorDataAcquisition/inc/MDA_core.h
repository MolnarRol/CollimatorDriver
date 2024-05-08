/**
 * @file MDA_core.h
 * @brief Motor data acquisition module
 * @details Module for readeading and evaluating measurement data for further regulation and control.
 *
 * =================================================================
 * @author Bc. Roland Molnar
 *
 * =================================================================
 * KEM, FEI, TUKE
 * @date 30.03.2024
 * @addtogroup MDA MDA core
 * @{
 */

#ifndef MODULES_MOTORDATAACQUISITION_INC_MDA_CORE_H_
#define MODULES_MOTORDATAACQUISITION_INC_MDA_CORE_H_

#include <MDA_config.h>
#include <MDA_interface.h>

/* Maximum ADC value. */
#define MDA_ADC_MAX_VALUE_dU16                          ( U16_MAX & ~(U16_MAX << MDA_ADC_RESOLUTION_BITS_dU16) )

/* Macro for calculating voltage from ADC reading */
#define MDA_ADC_VALUE_TO_VOLTS__V__dMF32(adc_val)       ( MDA_ADC_VOLTAGE_REF__V__dF32 * ((F32)adc_val / (F32)MDA_ADC_MAX_VALUE_dU16) )

/* Macro for calculating phase current from ADC reading. */
#define MDA_PHASE_CURRENT_FROM_ADC_VAL_dMF32(adc_val)    (MDA_ADC_VALUE_TO_VOLTS__V__dMF32(adc_val) - MDA_PHASE_CURRENT_ZERO_OFFSET__V__dF32) / MDA_PHASE_CURRENT_SENSITIVITY_dF32

/* Macro for calculating DC link voltage from ADC reading. */
#define MDA_DC_LINK_VOLTAGE_FROM_ADC_VAL_dMF32(adc_val) ( MDA_ADC_VALUE_TO_VOLTS__V__dMF32(adc_val) / MDA_DC_LINK_VOLTAGE_SENSITIVITY_dF32 )

/* ADC conversion results registers. */
#define MDA_ADC_U_CURRENT_CONV_RES_d                    ( AdcaResultRegs.ADCRESULT1 )
#define MDA_ADC_V_CURRENT_CONV_RES_d                    ( AdcbResultRegs.ADCRESULT0 )
#define MDA_ADC_W_CURRENT_CONV_RES_d                    ( AdcaResultRegs.ADCRESULT0 )
#define MDA_ADC_DC_LINK_CONV_RES_d                      ( AdcbResultRegs.ADCRESULT1 )

#define MDA_ENC_TIME_BETWEEN_PULSES__s__dMF32(count)    ( (F32)count / (F32)100.0E6 )

/* Initialization functions. */
static void MDA_AdcInit(void);
static void MDA_QepInit(void);

static void MDA_UpdateData(void);

/* Helper functions. */
static U16 MDA_GetRawRotorMechAngle_U16(void);
void MDA_GetRawPhaseCurrents(F32 * const u_pF32, F32 * const v_pF32, F32 * const w_pF32);

S32 MDA_delta_pos__pulses__S32();
F32 MDA_get_mech_speed_rads1_F32(S32 delta_pos__pulses__S32);

#endif /* MODULES_MOTORDATAACQUISITION_INC_MDA_CORE_H_ */

/**
 * @}
 */
