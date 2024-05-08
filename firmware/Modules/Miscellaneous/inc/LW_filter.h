/**
 * @file LW_filter.h
 * @brief Low pass filters for measured currents and speed
 * @details Filters measured values, discretized formability of the first order
 *
 * =================================================================
 * @author Bc. Vadym Holysh
 *
 * =================================================================
 * KEM, FEI, TUKE
 * @date 14.04.2024
 * @addgroup LW Low Pas filters
 * @{
 */

#ifndef MODULES_MISCELLANEOUS_INC_LW_FILTER_H_
#define MODULES_MISCELLANEOUS_INC_LW_FILTER_H_

#include "F2837xD_device.h"
#include <main.h>

#define SAMPLING_TIME__s__df32               ( (F32)50.0e-6f )                                                                /**< MACRO Ts - sampling time measured current*/
#define TIME_CONST_F__s__df32                ( (F32)( SAMPLING_TIME__s__df32 * 5 ) )                                          /**< MACRO Tf - filter time constant*/
#define A_CONST_df32                         ( TIME_CONST_F__s__df32 / ( TIME_CONST_F__s__df32 + SAMPLING_TIME__s__df32 ) )   /**< MACRO a = Tf/(Tf+Ts)*/

#define SAMPLING_TIME_W__s__df32             ( (F32)( SAMPLING_TIME__s__df32 * 5 ) )                                          /**< MACRO Tsw - sampling time measured rotor speed*/
#define TIME_CONST_W__s__df32                ( (F32)( SAMPLING_TIME_W__s__df32 * 5 ) )                                        /**< MACRO Tfw - filter time constant*/
#define W_CONST_df32                         ( TIME_CONST_W__s__df32 / ( TIME_CONST_W__s__df32 + SAMPLING_TIME_W__s__df32 ) ) /**< MACRO aw = Tfw/(Tfw+Tsw)*/

F32 LW_Filter_id_CalculateOutput(F32 MeasuredData_F32);

F32 LW_Filter_iq_CalculateOutput(F32 MeasuredData_F32);

F32 LW_Filter_Speed_CalculateOutput(F32 MeasuredData_F32);

#endif /* MODULES_MISCELLANEOUS_INC_LW_FILTER_H_ */

/**
 * @}
 */
