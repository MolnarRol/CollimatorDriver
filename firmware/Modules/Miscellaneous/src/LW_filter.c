/*
 * LW_filter.c
 *
 *  Created on: 4 апр. 2024 г.
 *      Author: vadym
 */

#include "LW_filter.h"

F32 LW_Filter_CalculateOutput(F32 MeasuredData_F32){

    static F32 Output_Filter_F32 = 0;
    Output_Filter_F32 = ( ( 1 - A_CONST_df32 ) * MeasuredData_F32 ) + (A_CONST_df32 * Output_Filter_F32);

    return Output_Filter_F32;
}

