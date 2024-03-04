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
#define ECOM_MSG_TIMEOUT__us__dU16  ( (U16)120 )
#define ECOM_MSC_TIMEOUT_TICKS_dU32 ( ATB_US_TO_TICKS_dM_U32(ECOM_MSG_TIMEOUT__us__dU16) )

typedef struct
{
    ECOM_ProtocolHeader_enum    packet_id;
    U16                         packet_size;
} ECOM_PacketHeader_struct;

typedef struct
{
    ECOM_PacketHeader_struct    header;
    U16*                        payload_pU16;
#if ( ECOM_CRC_ENABLED == 1 )
    U16                         crc8_U16;
#endif  /* ECOM_CRC_ENABLED */
} ECOM_Packet_struct;

typedef struct
{
    U16                         data_aU16[ECOM_BUFFER_SIZE_dU16];
    U16                         head_U16;
    U16                         tail_U16;
    struct
    {
        U16                     buffer_rdy_flag:1;
        U16                     __reserved: 15;
    }buffer_state_s;
} ECOM_Buffer_struct;

static U16  ECOM_ReadBuffer_U16(ECOM_Buffer_struct * const buffer_ps, U16 * dst_data_pU16, U16 read_size);
static void ECOM_WriteBuffer(ECOM_Buffer_struct * const buffer_ps, const U16 * data_pU16, U16 size_U16);
static void ECOM_ResetBuffer(ECOM_Buffer_struct * const buffer_ps);
static U16  ECOM_GetBufferSize_U16(const ECOM_Buffer_struct * const buffer_ps);

static void ECOM_ParsePacket(ECOM_Packet_struct * const parsed_packet_s, U16* packet_raw_U16, const U16 packet_size_U16);

static void ECOM_RxHandler(void);
static void ECOM_TxHandler(void);

#endif /* ECOM_CORE_H_ */
