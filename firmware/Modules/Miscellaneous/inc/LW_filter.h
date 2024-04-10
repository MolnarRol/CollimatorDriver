/*
 * LW_filter.h
 *
 *  Created on: 4 ���. 2024 �.
 *      Author: vadym
 */

#ifndef MODULES_MISCELLANEOUS_INC_LW_FILTER_H_
#define MODULES_MISCELLANEOUS_INC_LW_FILTER_H_

#include "F2837xD_device.h"
#include <main.h>

#define SAMPLING_TIME__s__df32 ( (F32)1.0f )
#define TIME_CONST_F__s__df32 ( (F32)1.0f )
#define A_CONST_df32 ( TIME_CONST_F__s__df32 / ( TIME_CONST_F__s__df32 + SAMPLING_TIME__s__df32 ) )

F32 LW_Filter_Current_CalculateOutput(F32 MasuredData_F32);

F32 LW_Filter_Speed_CalculateOutput(F32 MeasuredData_F32);

#endif /* MODULES_MISCELLANEOUS_INC_LW_FILTER_H_ */
