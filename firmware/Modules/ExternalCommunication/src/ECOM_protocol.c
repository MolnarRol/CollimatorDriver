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

static const U16 s_ECOM_msg_transfer_fail_response_aU16[] = {TRANSFER_ERR_e, 0x2};

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
            break;

        case ECOM_PROTO_SM_STATE_PROCESSING_dU16:
            s_ECOM_protocol_state_machine_state_U16 = ECOM_PROTO_SM_STATE_RESPONDING_dU16;
            ECOM_ParsePacket(&s_rx_packet, s_ECOM_rx_buffer_s.data_aU16, ECOM_GET_BUFFER_LEN_dM(&s_ECOM_rx_buffer_s));
            if( ECOM_ProtocolCheckMsg(&s_rx_packet, &s_ECOM_rx_buffer_s) != (U16)0 )
            {
                ECOM_TxRequest(s_ECOM_msg_transfer_fail_response_aU16, 2);
            }

        case ECOM_PROTO_SM_STATE_RESPONDING_dU16:
            ECOM_ResetBuffer(&s_ECOM_rx_buffer_s);

            SCI_SetRxEnableState(True_b);
            s_ECOM_protocol_state_machine_state_U16 = ECOM_PROTO_SM_STATE_IDLE_dU16;
            break;

        default:
        {
            break;
        }
    }
}

static U16 ECOM_ProtocolCheckMsg(const ECOM_Packet_struct * const parsed_packet_ps, const ECOM_Buffer_struct * const buffer_ps)
{
    U16 ret_status_code_U16 = (U16)0;
    if( parsed_packet_ps->header_s.packet_id_e != COMMAND_e &&
        parsed_packet_ps->header_s.packet_id_e != DATA_TRANSFER_e &&
        parsed_packet_ps->header_s.packet_id_e != HELLO_MSG_e )
    {
        ret_status_code_U16 = (U16)1;                                               /* Invalid header ID. */
    }
    else if(parsed_packet_ps->header_s.packet_size_U16 != buffer_ps->top_U16)
    {
        ret_status_code_U16 = (U16)2;                                               /* Invalid message length. */
    }
    else
    {
        /* Nothing to do. */
    }

    return ret_status_code_U16;
}

static void ECOM_ParsePacket(ECOM_Packet_struct * const parsed_packet_s, U16* packet_raw_U16, const U16 packet_size_U16)
{
    parsed_packet_s->header_s.packet_id_e = (ECOM_ProtocolHeader_enum)*(packet_raw_U16++);
    parsed_packet_s->header_s.packet_size_U16 = *(packet_raw_U16++);
    parsed_packet_s->payload_pU16 = packet_raw_U16;
#if ( ECOM_CRC_ENABLED == 1 )
    parsed_packet_s->crc8_U16 = *((U16*)packet_raw_U16 + (packet_size_U16 - 1));
#endif  /* ECOM_CRC_ENABLED */
}
