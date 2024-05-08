/**
 * @file ATB.c
 *
 * @section ATB ATB Application time base module
 * @brief Application time base module
 * @details Module for keeping up time. It can be used for precise enough non critical execution of functions in non blocking manner.
 *
 * =================================================================
 * @author Bc. Roland Molnar
 *
 * =================================================================
 * KEM, FEI, TUKE
 * @date 29.02.2024
 * @defgroup ATB Application time base module
 * @{
 */
#include <ATB_core.h>

static volatile U64 s_ATB_ticks_U64;                                /**< Application tick timer variable. */
static U16 s_ATB_Initialized_U16 = 0;                               /**< Module initialization status. */


/**
 * @brief   ATB module initialization function.
 * @details Configures timer to overflow with period of 10us to execute ATB_ISR.
 */
void ATB_Init(void)
{
    if(s_ATB_Initialized_U16 == (U16)0)                             /* Initialize only once. */
    {
        CpuTimer0Regs.TCR.bit.FREE = (U16)1;                        /* While debugging - do not stop the timer. */
        CpuTimer0Regs.TPRH.all = (U16)0;
        CpuTimer0Regs.TPR.bit.TDDR = (U16)19;                       /* Divide timer clock by 20. */
        CpuTimer0Regs.PRD.all = (U32)99;                            /* Timer overflow every 10us. */
        CpuTimer0Regs.TCR.bit.TIE = (U16)1;                         /* Enable timer overflow interrupt. */                                                  /* Enable interrupts. */
        s_ATB_Initialized_U16 = (U16)1;
    }

    CpuTimer1Regs.TCR.bit.FREE = (U16)1;                        /* While debugging - do not stop the timer. */
    CpuTimer1Regs.TPRH.all = (U16)0;
    CpuTimer1Regs.TPR.bit.TDDR = (U16)19;                       /* Divide timer clock by 20. */
    CpuTimer1Regs.PRD.all = (U32)990000;                            /* Timer overflow every 10us. */
    CpuTimer1Regs.TCR.bit.TIE = (U16)1;                         /* Enable timer overflow interrupt. */
    CpuTimer1Regs.TCR.bit.TSS = 0;
}

/**
 * @brief   Get current application time base ticks.
 * @return  Unsigned 32-bit integer value of tick counter.
 */
U64 ATB_GetTicks_U64(void)
{
    return s_ATB_ticks_U64;
}

/**
 * @brief   Get current application time base ticks.
 * @return  Unsigned 64-bit integer value of tick counter.
 */
U32 ATB_GetTicks_U32(void)
{
    return (U32)s_ATB_ticks_U64;
}

/**
 * @brief   Check if requested ticks (time) have passed.
 * @param   ref_ticks_U32 Ticks reference for check.
 * @param   checked_ticks_passed_U32 Required number of ticks elapsed. To calculate this value from time you can use this macros:
 *          @arg @ref ATB_US_TO_TICKS_dM_U32()
 *          @arg @ref ATB_MS_TO_TICKS_dM_U32()
 *          @arg @ref ATM_S_TO_TICKS_dM_U32()
 * @return  Time passed status.
 * @retval  1 number of ticks have passed
 * @retval  0 number of ticks have not passed
 *
 * <b>Usage example:</b>
 * @code{.c}
 * U32 time1_U32 = ATB_GetTicks_U32();
 * while(1)
 * {
 *      if( ATB_CheckTicksPassed_U16(time1_U32, ATB_MS_TO_TICKS_dM_U32(250)) )
 *      {
 *          function()                                      // Executed every 250ms.
 *          time1_U32 = ATB_GetTicks_U32();                 // Write new reference time
 *      }
 * }
 * @endcode
 */
U16  ATB_CheckTicksPassed_U16(const uint32_t ref_ticks_U32, const uint32_t checked_ticks_passed_U32)
{
    const U32 ticks_timestamp_U32 = ATB_GetTicks_U32();                             /* Saving current time stamp. */
    U32 ticks_delta_U32 = ticks_timestamp_U32 - ref_ticks_U32;                      /* Calculation of delta of ticks. */
    U16 status_U16 = (U16)0;                                                        /* Return value */

    if(ticks_delta_U32 >= checked_ticks_passed_U32)
    {
        status_U16 = (U16)1;
    }

    return status_U16;
}

void ATB_IncrementTime(void)
{
    s_ATB_ticks_U64 += (U64)1;
}
/**
 * @}
 */
