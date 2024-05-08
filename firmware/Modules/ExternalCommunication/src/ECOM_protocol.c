/**
 * @file ECOM_protocol.c
 * @brief External communication submodule.
 * @details Manages motor position requests and motor control states.
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
#include <ECOM_core.h>
#include <AC_interface.h>
#include <CRC.h>
#include <MTCL_interface.h>

U16 s_ECOM_protocol_state_machine_state_U16 = ECOM_PROTO_SM_STATE_IDLE_dU16;
ECOM_Packet_struct s_rx_packet;
static U16 s_response_data_aU16[64];
static U16 s_response_data_size_U16 = 0;

static U16 s_response_packet_aU16[64];

extern ECOM_Buffer_struct s_ECOM_rx_buffer_s;

static U16* s_response_paU16 = (U16*)0;


/**
 * @brief Callback for serial data reception.
 */
void ECOM_DataRecievedCallback(void)
{
    /* Check if protocol state machine is available. */
    if(s_ECOM_protocol_state_machine_state_U16 == ECOM_PROTO_SM_STATE_IDLE_dU16)
    {
        s_ECOM_protocol_state_machine_state_U16 = ECOM_PROTO_SM_STATE_PROCESSING_dU16;
        SCI_SetRxEnableState(False_b);
    }
}

/**
 * @brief Communication protocol state machine.
 */
void ECOM_ProtocolStateMachineHandler(void)
{
    switch(s_ECOM_protocol_state_machine_state_U16)
    {
        case ECOM_PROTO_SM_STATE_IDLE_dU16:
            break;

        case ECOM_PROTO_SM_STATE_PROCESSING_dU16:
            /* Parsing incoming packet. */
            ECOM_ParsePacket(&s_rx_packet, s_ECOM_rx_buffer_s.data_aU16, ECOM_GET_BUFFER_LEN_dM(&s_ECOM_rx_buffer_s));

            /* Checking if packet is OK. */
            if( ECOM_ProtocolCheckMsg(&s_rx_packet, &s_ECOM_rx_buffer_s) != (U16)0 )
            {
                ECOM_CreatePacket(TRANSFER_ERR_e, s_response_packet_aU16, 0, 0);
            }
            else
            {
                ECOM_ProtocolRespond(&s_rx_packet);

            }
            s_ECOM_protocol_state_machine_state_U16 = ECOM_PROTO_SM_STATE_RESPONDING_dU16;                             /* Set next state. */

        case ECOM_PROTO_SM_STATE_RESPONDING_dU16:
            if( ECOM_TxRequest(s_response_packet_aU16, s_response_packet_aU16[1]) != OK_e )
            {
                break;
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

/**
 * @brief External communication response control.
 * @param packet_ps is a destination buffer for created packet.
 */
static void ECOM_ProtocolRespond(const ECOM_Packet_struct * const packet_ps)
{
    switch(packet_ps->header_s.packet_id_e)
    {
        case HELLO_MSG_e:
        {
            s_response_data_aU16[0] = *(U16*)MTCL_GetControlState_ps();
            s_response_data_size_U16 = 1;
            ECOM_CreatePacket(HELLO_MSG_e, s_response_packet_aU16, s_response_data_aU16, s_response_data_size_U16);
            break;
        }
        case COMMAND_e:
        {
            AC_ExecuteCommand(packet_ps->payload_pU16,
                              packet_ps->header_s.packet_size_U16 - 3,
                              s_response_data_aU16,
                              &s_response_data_size_U16);
            ECOM_CreatePacket(COMMAND_RES_e, s_response_packet_aU16, s_response_data_aU16, s_response_data_size_U16);
            break;
        }
        default:
        {
            break;
        }
    }
}

/**
 * @brief Check incomming mesage from serial interface for errors.
 * @param parsed_packet_ps is pointer to parsed packet.
 * @param buffer_ps is a poitner to a raw rx buffer.
 * @returns Error value of check.
 * @retval 0 Packet OK.
 * @retval 1 Invalid header detected.
 * @retval 2 Received invalid length.
 * @retval 3 Checksum failed.
 */
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
    else if(CRC8_CCITT_Verify_b( &buffer_ps->data_aU16,
                                 parsed_packet_ps->header_s.packet_size_U16 - 1,
                                 parsed_packet_ps->crc8_U16) == False_b )
    {
        ret_status_code_U16 = (U16)3;
    }

    return ret_status_code_U16;
}

/**
 * @brief Parse incomming data as transmittion packet.
 * @param parsed_packet_s is poitner to parsed output.
 * @param packet_raw_U16 are the raw bytes received.
 * @param packet_size_U16 is the total number of received bytes.
 */
static void ECOM_ParsePacket(ECOM_Packet_struct * const parsed_packet_s, U16* packet_raw_U16, const U16 packet_size_U16)
{
    parsed_packet_s->header_s.packet_id_e = (ECOM_ProtocolHeader_enum)packet_raw_U16[0];
    parsed_packet_s->header_s.packet_size_U16 = packet_raw_U16[1];
    parsed_packet_s->payload_pU16 = &packet_raw_U16[2];
    parsed_packet_s->crc8_U16 = packet_raw_U16[packet_size_U16 - 1];
}

/**
 * @brief Create packet from parameters.
 * @param header_e is a header identification ID.
 * @param dst_buff_pU16 is a pointer to destination byte buffer for packet.
 * @param payload_pU16 is a pointer to raw data bytes to be included as payload.
 * @param payload_size_U16 is total number of bytes in payload buffer.
 */
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

/**
 * @}
 */
