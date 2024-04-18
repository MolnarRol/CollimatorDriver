/*
 * LW_filter.c
 *
 *  Created on: 4 ���. 2024 �.
 *      Author: vadym
 */

#include "LW_filter.h"

F32 LW_Filter_id_CalculateOutput(F32 MeasuredData_F32){

    static F32 Output_Filter_id_F32 = 0;
    Output_Filter_id_F32 = ( ( 1 - A_CONST_df32 ) * MeasuredData_F32 ) + (A_CONST_df32 * Output_Filter_id_F32);

    return Output_Filter_id_F32;
}

F32 LW_Filter_iq_CalculateOutput(F32 MeasuredData_F32){

    static F32 Output_Filter_iq_F32 = 0;
    Output_Filter_iq_F32 = ( ( 1 - A_CONST_df32 ) * MeasuredData_F32 ) + (A_CONST_df32 * Output_Filter_iq_F32);

    return Output_Filter_iq_F32;
}

F32 LW_Filter_Speed_CalculateOutput(F32 MeasuredData_F32){

    static F32 Output_Filter_speed_F32 = 0;
    Output_Filter_speed_F32 = ( ( 1 - W_CONST_df32 ) * MeasuredData_F32 ) + (W_CONST_df32 * Output_Filter_speed_F32);

    return Output_Filter_speed_F32;
}



