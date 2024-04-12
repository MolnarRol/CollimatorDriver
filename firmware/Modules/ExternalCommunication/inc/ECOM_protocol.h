/*
 * ECOM_PROTO.h
 *
 *  Created on: Mar 31, 2024
 *      Author: roland
 */
#include <app_types.h>
#include <ECOM_config.h>

#ifndef MODULES_EXTERNALCOMMUNICATION_INC_ECOM_PROTO_H_
#define MODULES_EXTERNALCOMMUNICATION_INC_ECOM_PROTO_H_

#define ECOM_PROTO_SM_STATE_IDLE_dU16           ( (U16)0 )
#define ECOM_PROTO_SM_STATE_PROCESSING_dU16     ( (U16)1 )
#define ECOM_PROTO_SM_STATE_RESPONDING_dU16     ( (U16)2 )
#define ECOM_PROTO_SM_STATE_RESPONDED_dU16      ( (U16)3 )

typedef enum
{
    PROTO_STATE_IDLE_e          = 0,
    PROTO_STATE_RESOLVING_e     = 1
} ECOM_Protocol_State_enum;

typedef enum
{
    COMMAND_e                   = 0x01,
    DATA_TRANSFER_e             = 0x02,
    COMMAND_RES_e               = 0x81,
    TRANSFER_ERR_e              = 0xFE,
    HELLO_MSG_e                 = 0xFF
} ECOM_ProtocolHeader_enum;

typedef struct
{
    ECOM_ProtocolHeader_enum    packet_id_e;
    U16                         packet_size_U16;
} ECOM_PacketHeader_struct;

typedef struct
{
    ECOM_PacketHeader_struct    header_s;
    U16*                        payload_pU16;
    U16                         crc8_U16;
} ECOM_Packet_struct;

void        ECOM_ProtocolStateMachineHandler(void);
static void ECOM_ParsePacket(ECOM_Packet_struct * const parsed_packet_s, U16* packet_raw_U16, const U16 packet_size_U16);
static U16  ECOM_ProtocolCheckMsg(const ECOM_Packet_struct * const parsed_packet_ps, const ECOM_Buffer_struct * const buffer_ps);
static U16* ECOM_ProtocolRespond_pU16(const ECOM_Packet_struct * const packet_ps);

#endif /* MODULES_EXTERNALCOMMUNICATION_INC_ECOM_PROTO_H_ */
