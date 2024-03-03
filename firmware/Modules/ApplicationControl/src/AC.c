/**
 * @file AC.c
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
#include <AC_core.h>
#include <AC_interface.h>

static U16 s_AC_active_function_U16 = (U16)0;
static U16 s_AC_request_active_U16 = (U16)0;

void AC_MainHandler(void)
{
    if(s_AC_request_active_U16 != (U16)0)
    {
        if(AC_CORE_CHECK_INDEX_BOUND_dM_b(s_AC_active_function_U16))
        {
            (AC_Funtions[s_AC_active_function_U16])(0, 0);
        }
        s_AC_request_active_U16 = (U16)0;
    }
}

void AC_SetRequest(const U16 request_function_U16)
{
    s_AC_request_active_U16 = (U16)1;
    s_AC_active_function_U16 = request_function_U16;

}

/* Application functions */
static AC_CoreStatus_enum AC_TestFunction(const void* const x, const U16 y)
{
    return AC_CORE_OK_e;
}

static AC_CoreStatus_enum AC_TestFunction2(const void* const x, const U16 y)
{
    return AC_CORE_OK_e;
}
