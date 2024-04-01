/*
 * ECOM_PROTO.c
 *
 *  Created on: Mar 31, 2024
 *      Author: roland
 */
#include <ECOM_core.h>

U16 s_ECOM_protocol_state_machine_state_U16 = ECOM_PROTO_SM_STATE_IDLE_dU16;
ECOM_Packet_struct s_rx_packet;

extern ECOM_Buffer_struct s_ECOM_rx_buffer_s;

static const U16 s_ECOM_msg_fail_response_aU16[] = {0xFF, 0x3, 0x2};

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
//            ECOM_ResetBuffer(&s_ECOM_rx_buffer_s);
            SCI_SetRxEnableState(True_b);
//            s_ECOM_protocol_state_machine_state_U16 = ECOM_PROTO_SM_STATE_PROCESSING_dU16;
            break;

        case ECOM_PROTO_SM_STATE_PROCESSING_dU16:
            s_ECOM_protocol_state_machine_state_U16 = ECOM_PROTO_SM_STATE_RESPONDING_dU16;
            ECOM_ParsePacket(&s_rx_packet, s_ECOM_rx_buffer_s.data_aU16, ECOM_GET_BUFFER_LEN_dM(&s_ECOM_rx_buffer_s));
            {
                ECOM_TxRequest(s_ECOM_msg_fail_response_aU16, 3);
            }

        case ECOM_PROTO_SM_STATE_RESPONDING_dU16:
            ECOM_ResetBuffer(&s_ECOM_rx_buffer_s);
            s_ECOM_protocol_state_machine_state_U16 = ECOM_PROTO_SM_STATE_IDLE_dU16;
            break;

        default:
        {
            break;
        }
    }
}

static void ECOM_ParsePacket(ECOM_Packet_struct * const parsed_packet_s, U16* packet_raw_U16, const U16 packet_size_U16)
{
    parsed_packet_s->header_s.packet_id_U16 = ((ECOM_PacketHeader_struct*)packet_raw_U16)->packet_id_U16;
    parsed_packet_s->header_s.packet_size_U16 = ((ECOM_PacketHeader_struct*)packet_raw_U16)->packet_size_U16;
    parsed_packet_s->payload_pU16 = (U16*)packet_raw_U16 + ( sizeof(ECOM_PacketHeader_struct) / sizeof(U16) );
#if ( ECOM_CRC_ENABLED == 1 )
    parsed_packet_s->crc8_U16 = *((U16*)packet_raw_U16 + (packet_size_U16 - 1));
#endif  /* ECOM_CRC_ENABLED */
}
