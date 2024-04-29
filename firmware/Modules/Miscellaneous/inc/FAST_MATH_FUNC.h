/**
 * @file FAST_MATH_FUNC.h
 * @brief Fast math functions
 * @details Fast approximations for trigonometric functions
 *
 * =================================================================
 * @author Bc. Roland Molnar
 *
 * =================================================================
 * KEM, FEI, TUKE
 * @date 07.04.2024
 * @addtogorup FM FM declarations
 * @{
 */
#ifndef FAST_MATH_FUNC_H
#define FAST_MATH_FUNC_H

#include <app_types.h>
#define TWO_PI_dF32                 ( (float)6.283185307179586f )

F32 FM_sin_F32(const F32 angle__rad__F32);
F32 FM_cos_F32(const F32 angle__rad__F32);
F32 FM_sqrt_F32(const F32 num_F32);
F32 FM_RemainderAfterFloatDivision_F32(const F32 value_F32, const F32 divisor_F32);
#endif /* FAST_MATH_FUNC_H */

/**
 * @}
 */
