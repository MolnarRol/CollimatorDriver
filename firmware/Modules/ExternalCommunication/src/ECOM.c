/*
 * ECOM.c
 *
 *  Created on: Mar 2, 2024
 *      Author: roland
 */
#include <ECOM_core.h>
#include <ECOM_interface.h>
#include <ECOM_buffers.h>

#include <ATB_interface.h>
#include <SCI.h>

/* Communication buffers. */
ECOM_Buffer_struct s_ECOM_rx_buffer_s = {0};
ECOM_Buffer_struct s_ECOM_tx_buffer_s = {0};

ECOM_Packet_struct rx_packet;


void ECOM_MainHandler(void)
{
    ECOM_RxHandler();
    ECOM_TxHandler();
}

static void ECOM_DataRecievedCallback(void)
{

}

/*
 * SCI Communication handlers.
 */

/**
 * @brief External communication receive handler
 * @details Used SCI FIFO in non interrupt manner. The function writes data from SCI RX FIFO to circular buffer.
 *          After time defined by @ref ECOM_MSC_TIMEOUT_TICKS_dU32 the data is considered as valid frame from external interface
 *          and can be further processed.
 *          Called by @ref ECOM_MainHandler()
 */
static void ECOM_RxHandler(void)
{
    static U16 s_bytes_recieved_flag_b = 0;                                                                          /* Some data was received over serial interface. */
    static U32 s_last_data_timestamp_U32;                                                                           /* Time stamp for new data reception. */

    if( SCI_RX_BYTES_AVAILABLE_dM != (U16)0)                                                                        /* Check RX FIFO buffer level. */
    {
        s_bytes_recieved_flag_b = 1;
        ECOM_WriteDataToBuffer(&s_ECOM_rx_buffer_s, (const U16*)&SCI_RX_BUFF_dM, (U16)SCI_RX_BYTES_AVAILABLE_dM);     /* Write buffer structure with new data. */
        s_last_data_timestamp_U32 = ATB_GetTicks_U32();                                                             /* Write new data timestamp. */
    }
    else if( ATB_CheckTicksPassed_U16(s_last_data_timestamp_U32, ECOM_MSC_TIMEOUT_TICKS_dU32)                       /* Check if msg timeout elapsed. */
             && s_bytes_recieved_flag_b )
    {
        ECOM_DataRecievedCallback();                                                                                /* Call data recieved callback. */
        s_ECOM_rx_buffer_s.buffer_state_s.buffer_rdy_flag = (U16)1;
        s_bytes_recieved_flag_b = 0;
    }
}

/**
 * @brief
 */
static void ECOM_TxHandler(void)
{
    if(s_ECOM_rx_buffer_s.buffer_state_s.buffer_rdy_flag)
    {
        ECOM_WriteBufferToAddress(&s_ECOM_rx_buffer_s, (U16*)&SCI_TX_BUFF_dM, SCI_TX_BYTES_AVAILABLE_dm);
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
