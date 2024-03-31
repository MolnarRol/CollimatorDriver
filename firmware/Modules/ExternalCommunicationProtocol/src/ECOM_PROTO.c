/*
 * ECOM_PROTO.c
 *
 *  Created on: Mar 31, 2024
 *      Author: roland
 */

//typedef struct
//{
//    ECOM_ProtocolHeader_enum    packet_id;
//    U16                         packet_size;
//} ECOM_PacketHeader_struct;
//
//typedef struct
//{
//    ECOM_PacketHeader_struct    header;
//    U16*                        payload_pU16;
//#if ( ECOM_CRC_ENABLED == 1 )
//    U16                         crc8_U16;
//#endif  /* ECOM_CRC_ENABLED */
//} ECOM_Packet_struct;

//typedef enum
//{
//    KeepAlive_e     = 0,
//    Command_e       = 1,
//    Status_e        = 2
//} ECOM_ProtocolHeader_enum;

//static void ECOM_ParsePacket(ECOM_Packet_struct * const parsed_packet_s, U16* packet_raw_U16, const U16 packet_size_U16)
//{
//    parsed_packet_s->header = *((ECOM_PacketHeader_struct*)packet_raw_U16);
//    parsed_packet_s->payload_pU16 = (U16*)packet_raw_U16 + ( sizeof(ECOM_PacketHeader_struct) / sizeof(U16) );
//#if ( ECOM_CRC_ENABLED == 1 )
//    parsed_packet_s->crc8_U16 = *((U16*)packet_raw_U16 + (packet_size_U16 - 1));
//#endif  /* ECOM_CRC_ENABLED */
//}
