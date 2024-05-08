/**
 * @file MDA_config.h
 * @brief Motor data acquisition module
 * @details Module for readeading and evaluating measurement data for further regulation and control.
 *
 * =================================================================
 * @author Bc. Roland Molnar
 *
 * =================================================================
 * KEM, FEI, TUKE
 * @date 30.03.2024
 * @addtogroup MDA MDA config
 * @{
 */

#ifndef MODULES_MOTORDATAACQUISITION_INC_MDA_CONFIG_H_
#define MODULES_MOTORDATAACQUISITION_INC_MDA_CONFIG_H_
#include <app_types.h>

/* Analog measurements defines. */
#define MDA_ADC_VOLTAGE_REF__V__dF32                    ( (F32)3.0 )            /**< ADC reference voltage. */
#define MDA_ADC_RESOLUTION_BITS_dU16                    ( (U16)12 )             /**< ADC resolution in bits. */
#define MDA_ADC_STARTUP_DELAY__us__U32                  ( (U32)1000 )           /**< Delay time after ADC power up. */
#define MDA_PHASE_CURRENT_SENSITIVITY_dF32              ( (F32)33.0E-3 )        /**< V / A */
#define MDA_PHASE_CURRENT_ZERO_OFFSET__V__dF32          ( (F32)1.5 )            /**< DC voltage offset marking 0 A. */
#define MDA_DC_LINK_VOLTAGE_SENSITIVITY_dF32            ( (F32)43.6E-3 )        /**< V / V */
#define MDA_ADC_CURRENT_MEASUREMENT_WINDOWS_dU16        ( (U16)19 )             /**< Number of CPU clock cycles for ADC sampling. */

#define MDA_PHASE_CURRENT_CALIBRATION_STEPS_U16         ( (U16)1000 )           /**< Number of steps for DC current sensor offset calibration. */
#define MDA_PHASE_CURRENT_CALIBRATION_STEP_DELAY__us__dU16 ( (U16)100 )         /**< Delay between calibration steps. */

/* Motor parameters. */
#define MOTOR_POLE_PAIRS_dU16                           ( (U16)4 )              /**< Number of motor pole pairs. */
#define MOTOR_LINEAR_TRANN_TRANSFER__rev_mm1__dF32      ( (F32)10 )             /**< Not used */

/* Speed and position defines. */
#define MDA_ENC_CPR_dU16                                ( (U16)4096 )           /* Encoder counts per revolution. */

#define MDA_ENC_SWAP_DIR_db                             ( False_b )             /* Swap A and B encoder channels. */

#define MDA_ENC_DELTA_PULSE_SAMPLE_TIME__s__dF32        ( (F32)2E-3 )
#define MDA_ENC_NO_PULSE_TIMEOUT__us__dU16              ( (U16)1900 )

#define SPEED_SAMPLE_TIMEdF32                           ( (F32)250E-6 )
#endif /* MODULES_MOTORDATAACQUISITION_INC_MDA_CONFIG_H_ */

/**
 * @}
 */
