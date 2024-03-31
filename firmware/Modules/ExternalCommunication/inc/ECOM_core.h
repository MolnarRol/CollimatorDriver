/*
 * ECOM_core.h
 *
 *  Created on: Mar 2, 2024
 *      Author: roland
 */
#include <app_types.h>
#include <ECOM_interface.h>

#ifndef ECOM_CORE_H_
#define ECOM_CORE_H_

#define ECOM_BUFFER_SIZE_dU16       ( (U16)64 )
#define ECOM_CRC_ENABLED            ( 0 )
#define ECOM_MSG_TIMEOUT__us__dU16  ( (U16)1000 )
#define ECOM_MSC_TIMEOUT_TICKS_dU32 ( ATB_US_TO_TICKS_dM_U32(ECOM_MSG_TIMEOUT__us__dU16) )



/* Communication handlers. */
static void ECOM_RxHandler(void);
static void ECOM_TxHandler(void);

//static void ECOM_ParsePacket(ECOM_Packet_struct * const parsed_packet_s, U16* packet_raw_U16, const U16 packet_size_U16);

#endif /* ECOM_CORE_H_ */
