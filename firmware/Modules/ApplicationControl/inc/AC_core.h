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

typedef enum
{
    AC_CORE_OK_e = 0,
    AC_CORE_FAIL_e
} AC_CoreStatus_enum;

typedef AC_CoreStatus_enum (*AC_ControlFunction_pF)(const void* const, const U16);

static AC_CoreStatus_enum AC_TestFunction(const void* const x, const U16 y);
static AC_CoreStatus_enum AC_TestFunction2(const void* const x, const U16 y);

static AC_ControlFunction_pF AC_Funtions[] =
{
     &AC_TestFunction,
     &AC_TestFunction2
};


#endif /* AC_CORE_H_ */
