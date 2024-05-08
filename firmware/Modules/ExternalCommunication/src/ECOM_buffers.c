/**
 * @file ECOM_buffers.c
 * @brief External communication buffer submodule.
 * @details Manages communication buffers.
 *
 * =================================================================
 * @author Bc. Roland Molnar
 *
 * =================================================================
 * KEM, FEI, TUKE
 * @date 29.02.2024
 * @addtogroup ECOM External communication protocol buffers
 * @{
 */
#include <ECOM_buffers.h>

/**
 * @brief Write data buffer to buffer structure
 * @param buffer_ps is a pointer to abstract buffer structure.
 * @param data_pU16 is a pointer to source byte buffer.
 * @param write_size_U16 is number of bytes to be transferred from source buffer to buffer structure.
 */
void ECOM_WriteDataToBuffer(ECOM_Buffer_struct * const buffer_ps, const U16 * data_pU16, U16 write_size_U16)
{
    while(write_size_U16 != (U16)0)
    {
        if(buffer_ps->top_U16 >= ECOM_BUFFER_SIZE_dU16)
        {
            ECOM_ResetBuffer(buffer_ps);
            break;
        }
        write_size_U16 -= (U16)1;
        buffer_ps->data_aU16[buffer_ps->top_U16] = *data_pU16;                           /* Copy data to the buffer. */
        data_pU16++;                                                                   /* Increment data pointer to the next value. */
        buffer_ps->top_U16 += (U16)1;                                                   /* Write new head index. */
    }
}

/**
 * @brief Write buffer to specified address.
 * @param buffer_ps pointer to buffer structure.
 * @param dst_data_pU16 is a destination address.
 * @param read_size is a number of bytes to be transfered from buffer to address.
 * @returns number of bytes left in buffer.
 */
U16 ECOM_WriteBufferToAddress(ECOM_Buffer_struct * const buffer_ps, U16 * const dst_data_pU16, U16 read_size)
{
    U16 buffer_len_U16 = ECOM_GET_BUFFER_LEN_dM(buffer_ps);
    while( (read_size != (U16)0) && (buffer_len_U16 != (U16)0) )
    {
        read_size -= (U16)1;
        *dst_data_pU16 = buffer_ps->data_aU16[buffer_ps->bottom_U16];
        buffer_ps->bottom_U16 += (U16)1;
        buffer_len_U16 = ECOM_GET_BUFFER_LEN_dM(buffer_ps);
    }
    return buffer_len_U16;
}

/**
 * @brief Reset communication buffer top and bottom index.
 */
void ECOM_ResetBuffer(ECOM_Buffer_struct * const buffer_ps)
{
    buffer_ps->bottom_U16 = (U16)0;
    buffer_ps->top_U16 = (U16)0;
}

/**
 * @}
 */

