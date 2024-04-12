/*
 * ECOM_PROTO.c
 *
 *  Created on: Mar 31, 2024
 *      Author: roland
 */
#include <ECOM_core.h>
#include <AC_interface.h>
#include <CRC.h>

U16 s_ECOM_protocol_state_machine_state_U16 = ECOM_PROTO_SM_STATE_IDLE_dU16;
ECOM_Packet_struct s_rx_packet;
static U16 s_response_data_aU16[64];

extern ECOM_Buffer_struct s_ECOM_rx_buffer_s;

/* Constant responses. */
static const U16 s_ECOM_msg_transfer_fail_response_aU16[] = {TRANSFER_ERR_e, 0x2};
static const U16 s_ECOM_msg_hello_response_aU16[] = {HELLO_MSG_e, 0x2};
static const U16 s_ECOM_msg_cmd_success_response_aU16[] = {COMMAND_RES_e, 0x3, 0x0};
static const U16 s_ECOM_msg_cmd_fail_response_aU16[] = {COMMAND_RES_e, 0x3, 0x1};

static U16* s_response_paU16 = (U16*)0;

void ECOM_DataRecievedCallback(void)
{
    /* Check if protocol state machine is available. */
    if(s_ECOM_protocol_state_machine_state_U16 == ECOM_PROTO_SM_STATE_IDLE_dU16)
    {
        s_ECOM_protocol_state_machine_state_U16 = ECOM_PROTO_SM_STATE_PROCESSING_dU16;
        SCI_SetRxEnableState(False_b);
    }
}

void ECOM_ProtocolStateMachineHandler(void)
{
    /*
    static U32 s_last_time_idle_state_ticks_U32 = (U32)0;
    if( ATB_CheckTicksPassed_U16(s_last_time_idle_state_ticks_U32, ATB_MS_TO_TICKS_dM_U32(50)) )
    {
        s_ECOM_protocol_state_machine_state_U16 = ECOM_PROTO_SM_STATE_RESPONDED_dU16;
        s_last_time_idle_state_ticks_U32 = ATB_GetTicks_U32();
    }
    */

    switch(s_ECOM_protocol_state_machine_state_U16)
    {
        case ECOM_PROTO_SM_STATE_IDLE_dU16:
//            s_last_time_idle_state_ticks_U32 = ATB_GetTicks_U32();
            break;

        case ECOM_PROTO_SM_STATE_PROCESSING_dU16:
            ECOM_ParsePacket(&s_rx_packet, s_ECOM_rx_buffer_s.data_aU16, ECOM_GET_BUFFER_LEN_dM(&s_ECOM_rx_buffer_s));   /* Parsing incoming packet. */

            if( ECOM_ProtocolCheckMsg(&s_rx_packet, &s_ECOM_rx_buffer_s) != (U16)0 )                                    /* Checking if packet is OK. */
            {
                s_response_paU16 = (U16*)s_ECOM_msg_transfer_fail_response_aU16;
            }
            else
            {
                s_response_paU16 = ECOM_ProtocolRespond_pU16(&s_rx_packet);
            }
            s_ECOM_protocol_state_machine_state_U16 = ECOM_PROTO_SM_STATE_RESPONDING_dU16;                             /* Set next state. */

        case ECOM_PROTO_SM_STATE_RESPONDING_dU16:
            if(s_response_paU16 != (U16*)0)
            {
                if( ECOM_TxRequest(s_response_paU16, s_response_paU16[1]) != OK_e )
                {
                    break;
                }
            }
            s_ECOM_protocol_state_machine_state_U16 = ECOM_PROTO_SM_STATE_RESPONDED_dU16;

        case ECOM_PROTO_SM_STATE_RESPONDED_dU16:
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

static U16* ECOM_ProtocolRespond_pU16(const ECOM_Packet_struct * const packet_ps)
{
    U16* resp_pU16 = (U16*)0;

    switch(packet_ps->header_s.packet_id_e)
    {
        case HELLO_MSG_e:
        {
            resp_pU16 = (U16*)s_ECOM_msg_hello_response_aU16;
            break;
        }
        case COMMAND_e:
        {
//            AC_ExecuteCommand_U16(packet_ps->payload_pU16, packet_ps->header_s.packet_size_U16 - 3);
            break;
        }
        default:
        {
            break;
        }
    }
    return resp_pU16;
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
    else if(CRC8_CCITT_Verify_b( buffer_ps->data_aU16,
                                 parsed_packet_ps->header_s.packet_size_U16,
                                 parsed_packet_ps->crc8_U16) == False_b )
    {
        ret_status_code_U16 = (U16)3;
    }
    else
    {
        /* Checking crc checksum. */

    }

    return ret_status_code_U16;
}

static void ECOM_ParsePacket(ECOM_Packet_struct * const parsed_packet_s, U16* packet_raw_U16, const U16 packet_size_U16)
{
    parsed_packet_s->header_s.packet_id_e = (ECOM_ProtocolHeader_enum)*(packet_raw_U16++);
    parsed_packet_s->header_s.packet_size_U16 = *(packet_raw_U16++);
    parsed_packet_s->payload_pU16 = packet_raw_U16;
    parsed_packet_s->crc8_U16 = *((U16*)packet_raw_U16 + (packet_size_U16 - 1));
}

static void ECOM_CreatePacket(const ECOM_ProtocolHeader_enum header_e, U16 * dst_buff_pU16, const U16 * payload_pU16, const U16 payload_size_U16)
{
    U16 * const dst_base_pU16 = dst_buff_pU16;
    U16 payload_iU16;

    /* Packet header. */
    *(dst_buff_pU16++) = (U16)header_e;
    *(dst_buff_pU16++) = (U16)payload_size_U16 + (U16)3;

    /* Payload. */
    for(payload_iU16 = (U16)0; payload_iU16 < payload_size_U16; payload_iU16++)
    {
        *(dst_buff_pU16++) = *(payload_pU16++);
    }

    /* Packet tail. */
    *(dst_buff_pU16) = CRC8_CCITT_U16(dst_base_pU16, payload_size_U16 + 2);
}
