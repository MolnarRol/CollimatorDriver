/*
 * MDA_config.h
 *
 *  Created on: Mar 13, 2024
 *      Author: roland
 */

#ifndef MODULES_MOTORDATAACQUISITION_INC_MDA_CONFIG_H_
#define MODULES_MOTORDATAACQUISITION_INC_MDA_CONFIG_H_
#include <app_types.h>

/* Analog measurements defines. */
#define MDA_ADC_VOLTAGE_REF__V__dF32                    ( (F32)3.0 )            /* ADC reference voltage. */
#define MDA_ADC_RESOLUTION_BITS_dU16                    ( (U16)12 )             /* ADC resolution in bits. */
#define MDA_ADC_STARTUP_DELAY__us__U32                  ( (U32)1000 )
#define MDA_PHASE_CURRENT_SENSITIVITY_dF32              ( (F32)33.0E-3 )        /* V / A */
#define MDA_PHASE_CURRENT_ZERO_OFFSET__V__dF32          ( (F32)1.5 )            /* DC voltage offset marking 0 A. */
#define MDA_DC_LINK_VOLTAGE_SENSITIVITY_dF32            ( (F32)43.6E-3 )        /* V / V */
#define MDA_ADC_CURRENT_MEASUREMENT_WINDOWS_dU16        ( (U16)19 )

/* Motor parameters. */
#define MOTOR_POLE_PAIRS_dU16                           ( (U16)4 )

/* Speed and position defines. */
#define MDA_ENC_CPR_dU16                                ( (U16)60 )             /* Encoder counts per revolution. */

#define MDA_ENC_SWAP_DIR_db                             ( False_b )             /* Swap A and B encoder channels. */

#define MDA_ENC_DELTA_PULSE_SAMPLE_TIME__s__dF32        ( (F32)250.0E-3 )
#define MDA_ENC_NO_PULSE_TIMEOUT__us__dU16              ( (U16)500 )

#endif /* MODULES_MOTORDATAACQUISITION_INC_MDA_CONFIG_H_ */
