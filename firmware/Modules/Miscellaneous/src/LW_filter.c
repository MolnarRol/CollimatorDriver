/**
 * @file LW_filter.c
 * @brief Low pass filters for measured currents and speed
 * @details Filters measured values, discretized formability of the first order
 *
 * =================================================================
 * @author Bc. Vadym Holysh
 *
 * =================================================================
 * KEM, FEI, TUKE
 * @date 14.04.2024
 * @defgroup LW Low Pas filters
 * @{
 */

#include "LW_filter.h"

/**
 * @brief   Low Pass filter for Id current.
 * @details Filters Id current.
 * @param Measured Id current.
 * @return Filtered value.
 */
F32 LW_Filter_id_CalculateOutput(F32 MeasuredData_F32){

    static F32 Output_Filter_id_F32 = 0;
    Output_Filter_id_F32 = ( ( 1 - A_CONST_df32 ) * MeasuredData_F32 ) + (A_CONST_df32 * Output_Filter_id_F32);

    return Output_Filter_id_F32;
}

/**
 * @brief   Low Pass filter for Iq current.
 * @details Filters Iq current.
 * @param Measured Iq current.
 * @return Filtered value.
 */
F32 LW_Filter_iq_CalculateOutput(F32 MeasuredData_F32){

    static F32 Output_Filter_iq_F32 = 0;
    Output_Filter_iq_F32 = ( ( 1 - A_CONST_df32 ) * MeasuredData_F32 ) + (A_CONST_df32 * Output_Filter_iq_F32);

    return Output_Filter_iq_F32;
}

/**
 * @brief   Low Pass filter for rotor speed.
 * @details Filters rotor speed.
 * @param Measured rotor speed.
 * @return Filtered value.
 */
F32 LW_Filter_Speed_CalculateOutput(F32 MeasuredData_F32){

    static F32 Output_Filter_speed_F32 = 0;
    Output_Filter_speed_F32 = ( ( 1 - W_CONST_df32 ) * MeasuredData_F32 ) + (W_CONST_df32 * Output_Filter_speed_F32);

    return Output_Filter_speed_F32;
}


/**
 * @}
 */
