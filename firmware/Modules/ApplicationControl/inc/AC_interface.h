/**
 * @file AC_interface.h
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

#ifndef INC_AC_INTERFACE_H_
#define INC_AC_INTERFACE_H_

typedef enum
{
    AC_OK_e = 0,
    AC_FAIL_e
} AC_RequestStatus_enum;

U16 AC_ExecuteCommand_U16(const U16 * const command_payload_pU16, const U16 payload_size_U16);

#endif /* INC_AC_INTERFACE_H_ */
