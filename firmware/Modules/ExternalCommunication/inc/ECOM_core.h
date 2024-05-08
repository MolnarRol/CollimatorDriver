/**
 * @file ECOM_core.h
 * @brief External communication buffer submodule.
 * @details Manages communication buffers.
 *
 * =================================================================
 * @author Bc. Roland Molnar
 *
 * =================================================================
 * KEM, FEI, TUKE
 * @date 29.02.2024
 * @addtogroup ECOM External communication protocol
 * @{
 */
#include <app_types.h>
#include <ATB_interface.h>
#include <SCI.h>

#include <ECOM_config.h>
#include <ECOM_buffers.h>
#include <ECOM_interface.h>
#include <ECOM_protocol.h>

#ifndef ECOM_CORE_H_
#define ECOM_CORE_H_

#define ECOM_BUFFER_SIZE_dU16       ( (U16)64 )                                                 /**< Size of Rx and Tx buffers. */
#define ECOM_MSG_TIMEOUT__us__dU16  ( (U16)1000 )                                               /**< Timeout for reception. */
#define ECOM_MSC_TIMEOUT_TICKS_dU32 ( ATB_US_TO_TICKS_dM_U32(ECOM_MSG_TIMEOUT__us__dU16) )      /**< Timeout for reception - ATB ticks. */


/* Communication handlers. */
static void ECOM_RxHandler(void);
static void ECOM_TxHandler(void);


#endif /* ECOM_CORE_H_ */

/**
 * @}
 */
