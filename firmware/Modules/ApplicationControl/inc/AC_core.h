/**
 * @file AC_core.h
 * @brief Application control module
 * @details Module for controlling application using user interface or external serial communication.
 *
 * =================================================================
 * @author Bc. Roland Molnar
 *
 * =================================================================
 * KEM, FEI, TUKE
 * @date 29.02.2024
 */
#include <app_types.h>

#ifndef AC_CORE_H_
#define AC_CORE_H_

#define AC_CORE_CHECK_INDEX_BOUND_dM_b(idx)   ( idx < (sizeof(AC_Funtions) / sizeof(AC_ControlFunction_pF)) )

typedef void (*AC_ControlFunction_pF)(const void* const, const U16, U16*, U16*);

static void AC_TestFunction(const void* const payload_p,
                            const U16 payload_size_U16,
                            U16 * response_data_pU16,
                            U16 * response_data_size_pU16);

static AC_ControlFunction_pF AC_Funtions[] =
{
     &AC_TestFunction
};


#endif /* AC_CORE_H_ */
