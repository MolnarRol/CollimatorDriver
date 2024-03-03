/*
 * ECOM.c
 *
 *  Created on: Mar 2, 2024
 *      Author: roland
 */
#include <ECOM_core.h>
#include <ATB_interface.h>
#include <SCI.h>

/* Communication buffers. */
ECOM_Buffer_struct s_ECOM_rx_buffer_s = {0};
ECOM_Buffer_struct s_ECOM_tx_buffer_s = {0};

ECOM_Packet_struct rx_packet;
U16 resp = 0;

void ECOM_MainHandler(void)
{
    ECOM_RxHandler();
    ECOM_TxHandler();
}

void ECOM_DataRecievedCallback()
{
    SCI_SendData(&s_ECOM_tx_buffer_s.data_aU16, s_ECOM_tx_buffer_s.buffer_state_s.level_U16);
}


/* SCI Communication handlers. */
static void ECOM_RxHandler(void)
{
    static U32 s_last_data_timestamp_U32;
    static U16 s_bytes_recieved_flag_b = 0;

    if(SciaRegs.SCIFFRX.bit.RXFFST != (U16)0)                                                                       /* Check RX FIFO buffer level. */
    {
        s_bytes_recieved_flag_b = 1;

        while(SciaRegs.SCIFFRX.bit.RXFFST)                                                                          /* Read data out of FIFO. */
        {
            ECOM_WriteBuffer(&s_ECOM_rx_buffer_s, (const U16*)&SciaRegs.SCIRXBUF, 1);                                 /* Write buffer structure with new data. */
        }
        s_last_data_timestamp_U32 = ATB_GetTicks_U32();                                                             /* Write new data timestamp. */
    }
    else if( ATB_CheckTicksPassed_U16(s_last_data_timestamp_U32, ECOM_MSC_TIMEOUT_TICKS_dU32)
             && s_bytes_recieved_flag_b )
    {
        ECOM_ClearBuffer(&s_ECOM_rx_buffer_s);
        ECOM_DataRecievedCallback();

        s_bytes_recieved_flag_b = 0;
    }
}

static void ECOM_TxHandler(void)
{

}

static void ECOM_ParsePacket(ECOM_Packet_struct * const parsed_packet_s, U16* packet_raw_U16, const U16 packet_size_U16)
{
    parsed_packet_s->header = *((ECOM_PacketHeader_struct*)packet_raw_U16);
    parsed_packet_s->payload_pU16 = (U16*)packet_raw_U16 + ( sizeof(ECOM_PacketHeader_struct) / sizeof(U16) );
#if ( ECOM_CRC_ENABLED == 1 )
    parsed_packet_s->crc8_U16 = *((U16*)packet_raw_U16 + (packet_size_U16 - 1));
#endif  /* ECOM_CRC_ENABLED */
}

static void ECOM_WriteBuffer(ECOM_Buffer_struct * const buffer_ps, const U16 * const data_pU16, const U16 size_U16)
{
    if( buffer_ps->buffer_state_s.buffer_overrun_flag_U16 )                                       /* Buffer overrun - do not write buffer. */
    {
        return;
    }
    U16 data_idx_U16;

    for(data_idx_U16 = (U16)0; data_idx_U16 < size_U16; data_idx_U16++ )
    {
        buffer_ps->data_aU16[buffer_ps->buffer_state_s.level_U16] = data_pU16[data_idx_U16];    /* Copy data to the buffer. */
        buffer_ps->buffer_state_s.level_U16 = (buffer_ps->buffer_state_s.level_U16 + (U16)1);

        if(buffer_ps->buffer_state_s.level_U16 >= ECOM_BUFFER_SIZE_dU16)                        /* Check buffer overrun. */
        {
            buffer_ps->buffer_state_s.buffer_overrun_flag_U16 = (U16)1;                           /* Set buffer overrun flag. */
            break;                                                                            /* Exit writing. */
        }
    }
}

static void ECOM_ClearBuffer(ECOM_Buffer_struct * const buffer_ps)
{
    buffer_ps->buffer_state_s.level_U16 = (U16)0;
}


