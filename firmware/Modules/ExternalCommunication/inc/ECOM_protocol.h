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

typedef enum
{
    PROTO_STATE_IDLE_e          = 0,
    PROTO_STATE_RESOLVING_e     = 1
} ECOM_Protocol_State_enum;

typedef enum
{
    KeepAlive_e                 = 0,
    Command_e                   = 1,
    Status_e                    = 2
} ECOM_ProtocolHeader_enum;

typedef struct
{
    U16                         packet_id_U16;
    U16                         packet_size_U16;
} ECOM_PacketHeader_struct;

typedef struct
{
    ECOM_PacketHeader_struct    header_s;
    U16*                        payload_pU16;
#if ( ECOM_CRC_ENABLED == 1 )
    U16                         crc8_U16;
#endif  /* ECOM_CRC_ENABLED */
} ECOM_Packet_struct;

void ECOM_ProtocolHandler(void);
static void ECOM_ParsePacket(ECOM_Packet_struct * const parsed_packet_s, U16* packet_raw_U16, const U16 packet_size_U16);


#endif /* MODULES_EXTERNALCOMMUNICATION_INC_ECOM_PROTO_H_ */
