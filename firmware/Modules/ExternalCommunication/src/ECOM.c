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
U16 test = 0;
U16 data;

void ECOM_MainHandler(void)
{
    DELAY_US(500);
    ECOM_RxHandler();
    ECOM_TxHandler();
}

void ECOM_DataRecievedCallback()
{
//    SCI_SendData(&s_ECOM_tx_buffer_s.data_aU16, s_ECOM_tx_buffer_s.buffer_state_s.level_U16);
//    ECOM_TxHandler();
}


/* SCI Communication handlers. */
/**
 * @brief External communication receive handler
 * @details Used SCI FIFO in non interrupt manner. The function writes data from SCI RX FIFO to circular buffer.
 *          After time defined by @ref ECOM_MSC_TIMEOUT_TICKS_dU32 the data is considered as valid frame from external interface
 *          and can be further processed.
 *          Called by @ref ECOM_MainHandler()
 */
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
    U16 fifo_free_bytes_U16;
    U16 tx_buffer_size_U16;

    if(s_ECOM_rx_buffer_s.buffer_state_s.buffer_rdy_flag)
    {
        fifo_free_bytes_U16 = (U16)16 - SciaRegs.SCIFFTX.bit.TXFFST;
        if(fifo_free_bytes_U16 != (U16)0)
        {
            tx_buffer_size_U16 = ECOM_ReadBuffer_U16(&s_ECOM_rx_buffer_s, &SciaRegs.SCITXBUF.all, fifo_free_bytes_U16);
        }
        if( tx_buffer_size_U16 == (U16)0 )
        {
            s_ECOM_rx_buffer_s.buffer_state_s.buffer_rdy_flag = 0;
        }
    }

}

static void ECOM_WriteBuffer(ECOM_Buffer_struct * const buffer_ps, const U16 * data_pU16, U16 write_size_U16)
{
    while(write_size_U16 != (U16)0)
    {
        write_size_U16 -= (U16)1;
        buffer_ps->data_aU16[buffer_ps->head_U16] = *data_pU16;                           /* Copy data to the buffer. */
        data_pU16++;                                                                    /* Increment data pointer to the next value. */
        buffer_ps->head_U16 = (buffer_ps->head_U16 + (U16)1) % ECOM_BUFFER_SIZE_dU16;     /* Write new head index. */
    }
}

static U16 ECOM_ReadBuffer_U16(ECOM_Buffer_struct * const buffer_ps, U16 * dst_data_pU16, U16 read_size)
{
    U16 buffer_len_U16 = ECOM_GetBufferSize_U16(buffer_ps);
    while( (read_size != (U16)0) && (buffer_len_U16 != (U16)0) )
    {
        read_size -= (U16)1;
        *dst_data_pU16 = buffer_ps->data_aU16[buffer_ps->tail_U16];
//        dst_data_pU16++;
        buffer_ps->tail_U16 = (buffer_ps->tail_U16 + (U16)1) % ECOM_BUFFER_SIZE_dU16;
        buffer_len_U16 = ECOM_GetBufferSize_U16(buffer_ps);
    }
    return buffer_len_U16;
}

static U16 ECOM_GetBufferSize_U16(const ECOM_Buffer_struct * const buffer_ps)
{
    U16 buffer_size_U16;
    if( buffer_ps->head_U16 >= buffer_ps->tail_U16 )
    {
        buffer_size_U16 = buffer_ps->head_U16 - buffer_ps->tail_U16;
    }
    else
    {
        buffer_size_U16 = ECOM_BUFFER_SIZE_dU16 - buffer_ps->tail_U16 + buffer_ps->head_U16;
    }

    return buffer_size_U16;
}

static void ECOM_ResetBuffer(ECOM_Buffer_struct * const buffer_ps)
{
    buffer_ps->tail_U16 = buffer_ps->head_U16;
}

static void ECOM_ParsePacket(ECOM_Packet_struct * const parsed_packet_s, U16* packet_raw_U16, const U16 packet_size_U16)
{
    parsed_packet_s->header = *((ECOM_PacketHeader_struct*)packet_raw_U16);
    parsed_packet_s->payload_pU16 = (U16*)packet_raw_U16 + ( sizeof(ECOM_PacketHeader_struct) / sizeof(U16) );
#if ( ECOM_CRC_ENABLED == 1 )
    parsed_packet_s->crc8_U16 = *((U16*)packet_raw_U16 + (packet_size_U16 - 1));
#endif  /* ECOM_CRC_ENABLED */
}

