/**
 * @file ECOM.c
 * @brief External communication submodule.
 * @details Manages motor position requests and motor control states.
 *
 * =================================================================
 * @author Bc. Roland Molnar
 *
 * =================================================================
 * KEM, FEI, TUKE
 * @date 29.02.2024
 * @defgroup ECOM External communication
 * @{
 */
#include <ECOM_core.h>

static boolean s_ATB_TxLock_b = False_b;

/* Communication buffers. */
ECOM_Buffer_struct s_ECOM_rx_buffer_s = {0};        /**< Receiver extended buffer structure. */
ECOM_Buffer_struct s_ECOM_tx_buffer_s = {0};        /**< Transmitter extended buffer structure. */

/**
 * @brief Communication handler.
 */
void ECOM_MainHandler(void)
{
    ECOM_RxHandler();
    ECOM_ProtocolStateMachineHandler();
    ECOM_TxHandler();
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
    static boolean s_bytes_recieved_flag_b = False_b;                                                                /* Some data was received over serial interface. */
    static U32 s_last_rx_timestamp_U32;                                                                           /* Time stamp for new data reception. */

    if(SCI_RX_BYTES_AVAILABLE_dMU16 != (U16)0)                                                                        /* Check RX FIFO buffer level. */
    {
        s_bytes_recieved_flag_b = True_b;
        ECOM_WriteDataToBuffer(&s_ECOM_rx_buffer_s, (const U16*)&SCI_RX_BUFF_dM, (U16)SCI_RX_BYTES_AVAILABLE_dMU16);     /* Write buffer structure with new data. */
        s_last_rx_timestamp_U32 = ATB_GetTicks_U32();                                                             /* Write new data timestamp. */
    }
    else if( ATB_CheckTicksPassed_U16(s_last_rx_timestamp_U32, ECOM_MSC_TIMEOUT_TICKS_dU32)                       /* Check if msg timeout elapsed. */
             && (s_bytes_recieved_flag_b == True_b) )
    {
        ECOM_DataRecievedCallback();                                                                                /* Call data recieved callback. */
        s_ECOM_rx_buffer_s.buffer_state_s.buffer_rdy_flag = (U16)1;
        s_bytes_recieved_flag_b = False_b;
    }
}

/**
 * @brief Transmit handler
 */
static void ECOM_TxHandler(void)
{
    static U32 last_tx_timestamp_U32;
    U16 buffer_bytes_left_U16;
    if(s_ECOM_tx_buffer_s.buffer_state_s.buffer_rdy_flag == (U16)1)
    {
        buffer_bytes_left_U16 = ECOM_WriteBufferToAddress(&s_ECOM_tx_buffer_s, (U16*)&SCI_TX_BUFF_dM, SCI_TX_BYTES_AVAILABLE_dMU16);
        if(buffer_bytes_left_U16 == (U16)0)
        {
            ECOM_ResetBuffer(&s_ECOM_tx_buffer_s);
            s_ECOM_tx_buffer_s.buffer_state_s.buffer_rdy_flag = (U16)0;
        }
        last_tx_timestamp_U32 = ATB_GetTicks_U32();
    }
    else
    {
        if(SCI_TX_BUFFER_EMPTY_dM && ATB_CheckTicksPassed_U16(last_tx_timestamp_U32, ECOM_MSC_TIMEOUT_TICKS_dU32))
        {
            s_ATB_TxLock_b = False_b;
        }
    }
}

/**
 * @brief Request transmit
 * @param data_pU16 is a pointer to source buffer.
 * @param data_len_U16 is a source buffer size.
 * @returns Request status
 */
ECOM_RequestStatus_enum ECOM_TxRequest(const U16* data_pU16, const U16 data_len_U16)
{
    ECOM_RequestStatus_enum ret_val_e = OK_e;

    if(s_ATB_TxLock_b != False_b)
    {
        ret_val_e = BUSY_e;
    }
    else if(data_len_U16 > ECOM_BUFFER_SIZE_dU16)
    {
        ret_val_e = ERR_e;
    }
    else
    {
        s_ATB_TxLock_b = True_b;
        ECOM_ResetBuffer(&s_ECOM_tx_buffer_s);
        ECOM_WriteDataToBuffer(&s_ECOM_tx_buffer_s, data_pU16, data_len_U16);
        s_ECOM_tx_buffer_s.buffer_state_s.buffer_rdy_flag = (U16)1;
    }

    return ret_val_e;
}

#pragma WEAK ( ECOM_DataRecievedCallback )
#pragma RETAIN ( ECOM_DataRecievedCallback )
void ECOM_DataRecievedCallback(void)
{
    /* Up to user implementation. */
}

/**
 * @}
 */
