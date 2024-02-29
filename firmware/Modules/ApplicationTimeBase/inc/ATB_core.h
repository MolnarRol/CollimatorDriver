#ifndef ATB_CORE_H
#define ATB_CORE_H
#include <ATB_interface.h>

/* Configs */
#define ATB_TIM_CLK__Hz__dU32       ( (U32)170E6 )                  /* Timer input clock. */
#define ATB_ISR_PRIO_dU32           ( (U32)14 )

void ATB_ISR(void);                                                /* Timer increment ISR. */

#endif  /* ATB_CORE_H */
