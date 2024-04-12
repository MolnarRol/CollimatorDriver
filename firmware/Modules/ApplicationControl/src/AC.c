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

void AC_ExecuteCommand( const U16 * const command_payload_pU16,
                        const U16 payload_size_U16,
                        U16 * response_data_pU16,
                        U16 * response_data_size_pU16 )
{
    if(AC_CORE_CHECK_INDEX_BOUND_dM_b(command_payload_pU16[0]))
    {
        AC_Funtions[command_payload_pU16[0]]( (command_payload_pU16 + 1),
                                              (payload_size_U16 - 1),
                                              response_data_pU16,
                                              response_data_size_pU16 );
    }
    else
    {
        *response_data_pU16 = 0;
        *response_data_size_pU16 = 0;
    }
}

/* Application functions */
static void AC_TestFunction(const void* const payload_p,
                            const U16 payload_size_U16,
                            U16 * response_data_pU16,
                            U16 * response_data_size_pU16)
{
    if(payload_size_U16 != 4)
    {
        return;
    }
    receive_data_U32 = BC_4BytesTo32BitData(payload_p).val_U32;
}
