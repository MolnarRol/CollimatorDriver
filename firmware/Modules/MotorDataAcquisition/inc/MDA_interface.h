/*
 * MDA_interface.h
 *
 *  Created on: Mar 13, 2024
 *      Author: roland
 */

#ifndef MODULES_MOTORDATAACQUISITION_INC_MDA_INTERFACE_H_
#define MODULES_MOTORDATAACQUISITION_INC_MDA_INTERFACE_H_
#include <main.h>
#include <MDA_config.h>

typedef struct
{
    F32 dc_link_voltage__V__F32;
    struct
    {
        F32 id__A__F32;
        F32 iq__A__F32;
    } currents_s;
    F32 rotor_el_angle__rad__F32;
    F32 rotor_el_speed__rpm__F32;
} MDA_Data_struct;

/* Macro for calculating voltage from ADC reading */
#define MDA_ADC_VALUE_TO_VOLTS__V__dMF32(adc_val)       ( MDA_ADC_VOLTAGE_REF__V__dF32 * ((F32)adc_val / (F32)MDA_ADC_MAX_VALUE_dU16) )

/**/
#define MDA_PHASE_CURRENT_FROM_ADC_VAL_dMF32(adc_val)   ( (MDA_ADC_VALUE_TO_VOLTS__V__dMF32(adc_val) - MDA_PHASE_CURRENT_ZERO_OFFSET__V__dF32) / MDA_PHASE_CURRENT_SENSITIVITY_dF32 )

/**/
#define MDA_DC_LINK_VOLTAGE_FROM_ADC_VAL_dMF32(adc_val) ( MDA_ADC_VALUE_TO_VOLTS__V__dMF32(adc_val) / MDA_DC_LINK_VOLTAGE_SENSITIVITY_dF32 )

void MDA_Init(void);
const MDA_Data_struct* MDA_GetData_ps(void);


#endif /* MODULES_MOTORDATAACQUISITION_INC_MDA_INTERFACE_H_ */
