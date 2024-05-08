/**
 * @file ATB_interface.h
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
 * @addtogroup ATB Application time base module interface
 * @{
 */
#ifndef ATB_IF_H
#define ATB_IF_H

#include <main.h>

/* Defines and macros */
#define     ATB_TICK__us__dU16              ( (U16)50 )                             /**< Period of one application time base tick. */

/* Time to ticks macros. */
#define     ATB_US_TO_TICKS_dM_U32(us)      ( (U32)((us) / ATB_TICK__us__dU16) )
#define     ATB_MS_TO_TICKS_dM_U32(ms)      ( (U32)((ms) * 1E3 / ATB_TICK__us__dU16) )
#define     ATM_S_TO_TICKS_dM_U32(s)        ( (U32)((s) * 1E6 /ATB_TICK__us__dU16) )

/* Ticks to time macros. */
#define     ATB_TICKS_TO_US_dM_F32(ticks)   ( (F32)((ticks) * ATB_TICK__us__dU16) )
#define     ATB_TICKS_TO_MS_dM_F32(ticks)   ( (F32)((ticks) * ATB_TICK__us__dU16) / 1.0E3f )
#define     ATB_TICKS_TO_S_dM_F32(ticks)    ( (F32)((ticks) * ATB_TICK__us__dU16) / 1.0E6f)

/* Interface functions. */
void        ATB_Init(void);
void        ATB_IncrementTime(void);
U64         ATB_GetTicks_U64(void);
U32         ATB_GetTicks_U32(void);
U16         ATB_CheckTicksPassed_U16(const uint32_t ref_ticks_U32, const uint32_t checked_ticks_passed_U32);

#endif  /* ATB_IF_H */

/**
 * @}
 */
