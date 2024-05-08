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
 * @addtogroup ATB Application time base module coree
 * @{
 */
#ifndef ATB_CORE_H
#define ATB_CORE_H
#include <ATB_interface.h>

/* Configs */
#define ATB_TIM_CLK__Hz__dU32       ( (U32)200E6 )                  /* Timer input clock. */

#endif  /* ATB_CORE_H */

/**
 * @}
 */
