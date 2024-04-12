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
#include <ByteConversions.h>


U32 receive_data_U32 = (U32)0;          /* Debug variable */

U16 AC_ExecuteCommand_U16(const U16 * const command_payload_pU16, const U16 payload_size_U16)
{
    U16 ret_val_U16 = (U16)0;
    if(AC_CORE_CHECK_INDEX_BOUND_dM_b(command_payload_pU16[0]))
    {
        ret_val_U16 = (U16)(AC_Funtions[command_payload_pU16[0]])((command_payload_pU16 + 1), (payload_size_U16 - 1));
    }
    return ret_val_U16;
}

/* Application functions */
static AC_CoreStatus_enum AC_TestFunction(const void* const x, const U16 y)
{
    if(y != 4)
    {
        return AC_CORE_FAIL_e;
    }
    receive_data_U32 = BC_4BytesTo32BitData(x).val_U32;
    return AC_CORE_OK_e;
}

static AC_CoreStatus_enum AC_TestFunction2(const void* const x, const U16 y)
{
    return AC_CORE_OK_e;
}
