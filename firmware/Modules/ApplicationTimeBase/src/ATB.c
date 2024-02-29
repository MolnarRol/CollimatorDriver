/**
 * @file ATB.c
 * @brief Application time base module
 * @details Details
 *
 * =================================================================
 * @author Bc. Roland Molnar
 *
 * =================================================================
 * KEM, FEI, TUKE
 * @date 29.02.2024
 */
#include <ATB_core.h>
//#include "TIM_UTIL.h"

static volatile U64 s_ATB_ticks_U64;                                                            /**< Application tick timer variable. */

/**
 * @defgroup ATB_InterfaceFunctions ATB Interface
 * @{
 */

/**
 * @brief   ATB module initialization function.
 * @param   time_base_period__us__U16 Period in microseconds for s_ATB_ticks_U64 increment.
 */
void ATB_Init(void)
{
//    /* Enable clocking. */
//    if(__HAL_RCC_TIM7_IS_CLK_ENABLED() == 0) __HAL_RCC_TIM7_CLK_ENABLE();
//
//    /* Basic timer setup. */
//    LL_TIM_DisableCounter(TIM7);
//    LL_TIM_SetCounter(TIM7, (U32)0);
//    s_ATB_ticks_U64 = (U64)0;
//
//    /* Timer parameter calculations. */
//    TIM_UTIL_Param_s timing_s = { .TIM_InputFreq__Hz__F32 = ATB_TIM_CLK__Hz__dU32 };
//    const F32 timer_freq__Hz__F32 = ( 1.0f/((F32)(ATB_TICK__us__dU16) * 1.0E-6F) );
//
//    if( TIM_UTIL_CalcSettingsFromFreq(&timing_s, timer_freq__Hz__F32) == TIM_UTIL_OK_e )
//    {
//        LL_TIM_SetPrescaler(TIM7, (U32)timing_s.PSC_U16);
//        LL_TIM_SetAutoReload(TIM7, timing_s.ARR_U32);
//    }
//
//    /* Interrupt setup. */
//    LL_TIM_EnableIT_UPDATE(TIM7);
//    NVIC_ClearPendingIRQ(TIM7_DAC_IRQn);
//    NVIC_SetPriority(TIM7_DAC_IRQn, ATB_ISR_PRIO_dU32);
//    NVIC_EnableIRQ(TIM7_DAC_IRQn);
//
//    /* Enable counter. */
//    LL_TIM_EnableCounter(TIM7);
}

/**
 * @brief   Get current application time base ticks.
 * @retval  Unsigned 32-bit integer value of tick counter.
 */
U64 ATB_GetTicks_U64(void)
{
    return s_ATB_ticks_U64;
}

/**
 * @brief   Get current application time base ticks.
 * @retval  Unsigned 64-bit integer value of tick counter.
 */
U32 ATB_GetTicks_U32(void)
{
    return (U32)s_ATB_ticks_U64;
}

/**
 * @brief   Check if requested ticks (time) have passed.
 * @param   ref_ticks_U32 Ticks reference for check.
 * @param   checked_ticks_passed_U32 Required number of ticks elapsed.
 * @retval  Returns 1 if requested number of ticks have passed, else returns 0.
 */
U16  ATB_CheckTicksPassed_U16(const uint32_t ref_ticks_U32, const uint32_t checked_ticks_passed_U32)
{
    const U32 ticks_timestamp_U32 = ATB_GetTicks_U32();                           /* Saving current time stamp. */
    U32 ticks_delta_U32 = ticks_timestamp_U32 - ref_ticks_U32;                  /* Calculation of delta of ticks. */
    U16 status_U16 = (U16)0;                                                       /* Return value */

    if(ticks_delta_U32 >= checked_ticks_passed_U32)
    {
        status_U16 = (U16)1;
    }

    return status_U16;
}

/**
 * @}
 */

/**
 * @brief   Interrupt service routine for incrementing the s_ATB_ticks_U64 variable.
 */
interrupt void ATB_ISR(void)
{
    s_ATB_ticks_U64 += (U64)1;                                                  /* Increment tick variable. */
    /* Reset interrupt flag. */

}




