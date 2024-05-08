/**
 * @file FAST_MATH_FUNC.c
 * @brief Fast math functions
 * @details Fast approximations for trigonometric functions
 *
 * =================================================================
 * @author Bc. Roland Molnar
 *
 * =================================================================
 * KEM, FEI, TUKE
 * @date 07.04.2024
 * @defgroup FM Fast math fuctions
 * @{
 */
#include <FAST_MATH_FUNC.h>

#define LUT_SIZE_d                  ( 64UL )                                                                                                /**< Number of points in look up table. */
#define LUT_ANGLE_STEP__rad__dF32   ( TWO_PI_dF32 / (float)LUT_SIZE_d )                                                                      /**< Delta in angle between points. */
#define SQRT_NEWTON_ITERATIONS_dU16 ( (U16)6 )

/* Static lookup tables */
const static F32 s_FM_SIN_LOOKUP_TABLE_F32[LUT_SIZE_d] =                                                                                    /**< Lookup table for cosine from [0, 2Pi] interval */
{
    0.00000000f,   0.09801714f,   0.19509032f,   0.29028468f,   0.38268343f,   0.47139674f,   0.55557023f,   0.63439328f,
    0.70710678f,   0.77301045f,   0.83146961f,   0.88192126f,   0.92387953f,   0.95694034f,   0.98078528f,   0.99518473f,
    1.00000000f,   0.99518473f,   0.98078528f,   0.95694034f,   0.92387953f,   0.88192126f,   0.83146961f,   0.77301045f,
    0.70710678f,   0.63439328f,   0.55557023f,   0.47139674f,   0.38268343f,   0.29028468f,   0.19509032f,   0.09801714f,
    0.00000000f,   -0.09801714f,  -0.19509032f,  -0.29028468f,  -0.38268343f,  -0.47139674f,  -0.55557023f,  -0.63439328f,
    -0.70710678f,  -0.77301045f,  -0.83146961f,  -0.88192126f,  -0.92387953f,  -0.95694034f,  -0.98078528f,  -0.99518473f,
    -1.00000000f,  -0.99518473f,  -0.98078528f,  -0.95694034f,  -0.92387953f,  -0.88192126f,  -0.83146961f,  -0.77301045f,
    -0.70710678f,  -0.63439328f,  -0.55557023f,  -0.47139674f,  -0.38268343f,  -0.29028468f,  -0.19509032f,  -0.09801714f
};

const static F32 s_FM_COS_LOOKUP_TABLE_F32[LUT_SIZE_d] =                                                                                   /**< Lookup table for cosine from [0, 2Pi] interval */
{
    1.00000000f,   0.99518473f,   0.98078528f,   0.95694034f,   0.92387953f,   0.88192126f,   0.83146961f,   0.77301045f,
    0.70710678f,   0.63439328f,   0.55557023f,   0.47139674f,   0.38268343f,   0.29028468f,   0.19509032f,   0.09801714f,
    0.00000000f,   -0.09801714f,  -0.19509032f,  -0.29028468f,  -0.38268343f,  -0.47139674f,  -0.55557023f,  -0.63439328f,
    -0.70710678f,  -0.77301045f,  -0.83146961f,  -0.88192126f,  -0.92387953f,  -0.95694034f,  -0.98078528f,  -0.99518473f,
    -1.00000000f,  -0.99518473f,  -0.98078528f,  -0.95694034f,  -0.92387953f,  -0.88192126f,  -0.83146961f,  -0.77301045f,
    -0.70710678f,  -0.63439328f,  -0.55557023f,  -0.47139674f,  -0.38268343f,  -0.29028468f,  -0.19509032f,  -0.09801714f,
    -0.00000000f,  0.09801714f,   0.19509032f,   0.29028468f,   0.38268343f,   0.47139674f,   0.55557023f,   0.63439328f,
    0.70710678f,   0.77301045f,   0.83146961f,   0.88192126f,   0.92387953f,   0.95694034f,   0.98078528f,   0.99518473f
};

/**
 * @brief Calculate remainder after floating point division
 * @details Modulo operation is available for integer numbers only. This functions enables similar functionality for floating point numbers.
 * @param value_F32 is a value that we want to divide
 * @param divisor_F32 is the divisor for the value_F32
 * @returns remainder after division
 */
F32 FM_RemainderAfterFloatDivision_F32(const F32 value_F32, const F32 divisor_F32)
{
    const F32 div_floor_F32 = (F32)( (uint32_t)(value_F32 / divisor_F32) );
    return value_F32 - div_floor_F32 * divisor_F32;
}

#pragma FUNC_ALWAYS_INLINE ( FM_TrigonometricLinearInterpolationFromLookupTable_F32 )
static inline F32 FM_TrigonometricLinearInterpolationFromLookupTable_F32(const F32 * const lookup_table_F32, const F32 angle__rad__F32)
{
    /* Mapping input angle into a [0, 2PI] interval. */
    F32 angle_normalised_F32 = FM_RemainderAfterFloatDivision_F32(angle__rad__F32, TWO_PI_dF32);
    if(angle_normalised_F32 < (F32)0.0)
    {
        angle_normalised_F32 += TWO_PI_dF32;
    }

    /* Getting values from lookup table. */
    const U16 lut_index_U16 = (U16)( angle_normalised_F32 / LUT_ANGLE_STEP__rad__dF32) % (U16)LUT_SIZE_d;                                   /* Calculating lower index of the look up table. */
    const F32 lut_lower_angle_F32 = lookup_table_F32[lut_index_U16];                                                                        /* Getting lower value from interval. */
    const F32 lut_higher_angle_F32 = lookup_table_F32[(lut_index_U16 + (U16)1) % (U16)LUT_SIZE_d];                                          /* Getting higher value from interval. */

    /* Approximation, */
    const F32 linear_slope_F32 = (lut_higher_angle_F32 - lut_lower_angle_F32) / LUT_ANGLE_STEP__rad__dF32;                                  /* Linear line slope calculation. */
    const F32 distance_from_lower_angle_F32 = FM_RemainderAfterFloatDivision_F32(angle_normalised_F32, LUT_ANGLE_STEP__rad__dF32);          /* Distance in x axis (angle) from lower interval value. */

    return ( linear_slope_F32 * distance_from_lower_angle_F32 + lut_lower_angle_F32);
}

/**
 * @brief Fast sine approximation
 * @details Sine is calculated from from look up table with linear interpolation between points.
 * @param angle__rad__F32 is the input angle in radians.
 * @returns sine value for input angle
 */
F32 FM_sin_F32(const F32 angle__rad__F32)
{
    return FM_TrigonometricLinearInterpolationFromLookupTable_F32(s_FM_SIN_LOOKUP_TABLE_F32, angle__rad__F32);
}

/**
 * @brief Fast cosine approximation
 * @details Cosine is calculated from from look up table with linear interpolation between points.
 * @param angle__rad__F32 is the input angle in radians.
 * @returns cosine value for input angle
 */
F32 FM_cos_F32(const F32 angle__rad__F32)
{
    return FM_TrigonometricLinearInterpolationFromLookupTable_F32(s_FM_COS_LOOKUP_TABLE_F32, angle__rad__F32);
}

/**
 * @brief Fast square root approximation
 * @details This function uses Newtonian iteration to approximate solution. https://en.wikipedia.org/wiki/Newton%27s_method
 * @param num_F32 is the positive number of which square root needs to be calculated.
 * @returns square root of the input number.
 */
F32 FM_sqrt_F32(const F32 num_F32)
{
    if(num_F32 < (F32)0.0)
    {
        return (F32)0.0;
    }

    U16 iter_iU16;
    F32 sqrt_F32;

    for(iter_iU16 = 0; iter_iU16 < 32; iter_iU16++)
    {
        sqrt_F32 = (F32)((U32)1 << (U32)iter_iU16);
        if(sqrt_F32 >= num_F32)
        {
            break;
        }
    }

    /* Newtonian iteration. */
    for(iter_iU16 = 0; iter_iU16 < SQRT_NEWTON_ITERATIONS_dU16; iter_iU16++)
    {
        sqrt_F32 = (F32)0.5 * (sqrt_F32 + num_F32 / sqrt_F32);
    }

    return sqrt_F32;
}

/**
 * @}
 */
