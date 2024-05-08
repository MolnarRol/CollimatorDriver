/**
 * @file ECOM_inetface.h
 * @brief External communication buffer submodule.
 * @details Manages communication buffers.
 *
 * =================================================================
 * @author Bc. Roland Molnar
 *
 * =================================================================
 * KEM, FEI, TUKE
 * @date 29.02.2024
 * @addtogroup ECOM External communication interface
 * @{
 */
#include <app_types.h>

#ifndef ECOM_INTERFACE_H_
#define ECOM_INTERFACE_H_

typedef enum
{
    OK_e    = 0,
    BUSY_e  = 1,
    ERR_e   = 2
} ECOM_RequestStatus_enum;


void ECOM_MainHandler(void);
ECOM_RequestStatus_enum ECOM_TxRequest(const U16* data_pU16, const U16 data_len_U16);
void ECOM_DataRecievedCallback(void);

#endif /* ECOM_INTERFACE_H_ */

/**
 * @}
 */
