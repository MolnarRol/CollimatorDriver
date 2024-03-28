/*
 * MDA_core.h
 *
 *  Created on: Mar 13, 2024
 *      Author: roland
 */

#ifndef MODULES_MOTORDATAACQUISITION_INC_MDA_CORE_H_
#define MODULES_MOTORDATAACQUISITION_INC_MDA_CORE_H_

#include <MDA_config.h>
#include <MDA_interface.h>

/* Maximum ADC value. */
#define MDA_ADC_MAX_VALUE_dU16                          ( U16_MAX & ~(U16_MAX << MDA_ADC_RESOLUTION_BITS_dU16) )

/* Macro for calculating voltage from ADC reading */
#define MDA_ADC_VALUE_TO_VOLTS__V__dMF32(adc_val)       ( MDA_ADC_VOLTAGE_REF__V__dF32 * ((F32)adc_val / (F32)MDA_ADC_MAX_VALUE_dU16) )

/**/
#define MDA_PHASE_CURRENT_FROM_ADC_VAL_dMF32(adc_val)   ( (MDA_ADC_VALUE_TO_VOLTS__V__dMF32(adc_val) - MDA_PHASE_CURRENT_ZERO_OFFSET__V__dF32) / MDA_PHASE_CURRENT_SENSITIVITY_dF32 )

/**/
#define MDA_DC_LINK_VOLTAGE_FROM_ADC_VAL_dMF32(adc_val) ( MDA_ADC_VALUE_TO_VOLTS__V__dMF32(adc_val) / MDA_DC_LINK_VOLTAGE_SENSITIVITY_dF32 )

/* ADC conversion results */
#define MDA_ADC_U_CURRENT_CONV_RES_d                    ( AdcaResultRegs.ADCRESULT1 )
#define MDA_ADC_V_CURRENT_CONV_RES_d                    ( AdcbResultRegs.ADCRESULT0 )
#define MDA_ADC_W_CURRENT_CONV_RES_d                    ( AdcaResultRegs.ADCRESULT0 )
#define MDA_ADC_DC_LINK_CONV_RES_d                      ( AdcbResultRegs.ADCRESULT1 )


static void MDA_AdcInit(void);
static void MDA_QepInit(void);
static void MDA_UpdateData(void);


#endif /* MODULES_MOTORDATAACQUISITION_INC_MDA_CORE_H_ */
