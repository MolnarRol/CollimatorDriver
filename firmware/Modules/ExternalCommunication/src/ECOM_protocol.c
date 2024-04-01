/*
 * ECOM_PROTO.c
 *
 *  Created on: Mar 31, 2024
 *      Author: roland
 */
#include <ECOM_core.h>

U16 s_ECOM_protocol_state_machine_state_U16 = ECOM_PROTO_SM_STATE_IDLE_dU16;

void ECOM_DataRecievedCallback(void)
{
    /* Check if protocol state machine is available. */
    if(s_ECOM_protocol_state_machine_state_U16 == ECOM_PROTO_SM_STATE_IDLE_dU16)
    {
        s_ECOM_protocol_state_machine_state_U16 = ECOM_PROTO_SM_STATE_PROCESSING_dU16;
        SCI_SetRxEnableState(False_b);
    }
}

void ECOM_ProtocolHandler(void)
{
    switch(s_ECOM_protocol_state_machine_state_U16)
    {
        case ECOM_PROTO_SM_STATE_IDLE_dU16:
        {
            SCI_SetRxEnableState(True_b);
//            s_ECOM_protocol_state_machine_state_U16 = ECOM_PROTO_SM_STATE_PROCESSING_dU16;
            break;
        }

        case ECOM_PROTO_SM_STATE_PROCESSING_dU16:
        {
            s_ECOM_protocol_state_machine_state_U16 = ECOM_PROTO_SM_STATE_RESPONDING_dU16;
            break;
        }

        case ECOM_PROTO_SM_STATE_RESPONDING_dU16:
        {
            s_ECOM_protocol_state_machine_state_U16 = ECOM_PROTO_SM_STATE_IDLE_dU16;
            break;
        }

        default:
        {
            break;
        }
    }
}





//static void ECOM_ParsePacket(ECOM_Packet_struct * const parsed_packet_s, U16* packet_raw_U16, const U16 packet_size_U16)
//{
//    parsed_packet_s->header = *((ECOM_PacketHeader_struct*)packet_raw_U16);
//    parsed_packet_s->payload_pU16 = (U16*)packet_raw_U16 + ( sizeof(ECOM_PacketHeader_struct) / sizeof(U16) );
//#if ( ECOM_CRC_ENABLED == 1 )
//    parsed_packet_s->crc8_U16 = *((U16*)packet_raw_U16 + (packet_size_U16 - 1));
//#endif  /* ECOM_CRC_ENABLED */
//}
