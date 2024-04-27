/*
 * ECOM_buffers.c
 *
 *  Created on: Mar 4, 2024
 *      Author: roland
 */
#include <ECOM_buffers.h>

void ECOM_WriteDataToBuffer(ECOM_Buffer_struct * const buffer_ps, const U16 * data_pU16, U16 write_size_U16)
{
    while(write_size_U16 != (U16)0)
    {
        if(buffer_ps->top_U16 >= ECOM_BUFFER_SIZE_dU16) break;
        write_size_U16 -= (U16)1;
        buffer_ps->data_aU16[buffer_ps->top_U16] = *data_pU16;                           /* Copy data to the buffer. */
        data_pU16++;                                                                   /* Increment data pointer to the next value. */
        buffer_ps->top_U16 += (U16)1;                                                   /* Write new head index. */
    }
}

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

void ECOM_ResetBuffer(ECOM_Buffer_struct * const buffer_ps)
{
    buffer_ps->bottom_U16 = (U16)0;
    buffer_ps->top_U16 = (U16)0;
}

