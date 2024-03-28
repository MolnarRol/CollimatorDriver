/*
 * MDA_config.h
 *
 *  Created on: Mar 13, 2024
 *      Author: roland
 */

#ifndef MODULES_MOTORDATAACQUISITION_INC_MDA_CONFIG_H_
#define MODULES_MOTORDATAACQUISITION_INC_MDA_CONFIG_H_
#include <app_types.h>

#define MDA_ADC_VOLTAGE_REF__V__dF32                    ( (F32)3.0 )                                            /* ADC reference voltage. */
#define MDA_ADC_RESOLUTION_BITS_dU16                    ( (U16)12 )                                             /* ADC resolution in bits. */
#define MDA_ADC_STARTUP_DELAY__us__U32                  ( (U32)1000 )
#define MDA_PHASE_CURRENT_SENSITIVITY_dF32              ( (F32)33.0E-3 )                                        /* V / A */
#define MDA_PHASE_CURRENT_ZERO_OFFSET__V__dF32          ( (F32)1.5 )                                            /* DC voltage offset marking 0 A. */
#define MDA_DC_LINK_VOLTAGE_SENSITIVITY_dF32            ( (F32)43.6E-3 )                                        /* V / V */

#endif /* MODULES_MOTORDATAACQUISITION_INC_MDA_CONFIG_H_ */
