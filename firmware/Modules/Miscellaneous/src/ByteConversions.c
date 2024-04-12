/*
 * ByteConversions.c
 *
 *  Created on: Apr 12, 2024
 *      Author: roland
 */
#include <ByteConversions.h>

/**
 * @brief
 */
#pragma FUNC_ALWAYS_INLINE ( BC_2BytesTo16BitData )
inline Int16Bits_struct BC_2BytesTo16BitData(const void *restrict const bytes_p)
{
    Int16Bits_struct ret_s = {0};
    ret_s.val_U16 = *(U16*)bytes_p << (U16)8;                                           /* Write the upper byte. */
    ret_s.val_U16 |= *((U16*)bytes_p + sizeof(U16)) & (U16)0x00FF;                      /* Write the lower byte. */
    return ret_s;
}

/**
 *
 */
#pragma FUNC_ALWAYS_INLINE ( BC_4BytesTo32BitData )
inline Int32Bits_struct BC_4BytesTo32BitData(const void *restrict const bytes_p)
{
    /* Not worth commenting :D. Just pointer tricks. */
    Int32Bits_struct ret_s = {0};
    ret_s.val_U32 = *((U32*)bytes_p) << (U32)24;
    ret_s.val_U32 |= (U32)(*((U16*)bytes_p + sizeof(U16)) & (U16)0x00FF) << (U32)16;
    ret_s.val_U32 |= (U32)(*((U16*)bytes_p + 2 * sizeof(U16)) & (U16)0x00FF) << (U32)8;
    ret_s.val_U32 |= (U32)(*((U16*)bytes_p + 3 * sizeof(U16)) & (U16)0x00FF);
    return ret_s;
}

/**
 *
 */
#pragma FUNC_ALWAYS_INLINE ( BC_16BitDataTo2Bytes )
inline void BC_16BitDataTo2Bytes(const void * const val_p, U16 *restrict output_bytes_pU16)
{
    output_bytes_pU16[1] = *(U16*)val_p & (U16)0x00FF;
    output_bytes_pU16[0] = *(U16*)val_p >> (U16)8;
}

/**
 *
 */
#pragma FUNC_ALWAYS_INLINE ( BC_32BitDataTo4Bytes )
inline void BC_32BitDataTo4Bytes(const void * const val_p, U16 *restrict output_bytes_pU16)
{
    U32 byte_i_U32;
    for(byte_i_U32 = (U32)0; byte_i_U32 < (U32)4; byte_i_U32++)
    {
        output_bytes_pU16[3 - byte_i_U32] = (*(U32*)val_p >> (U32)8 * byte_i_U32) & (U32)0x000000FF;
    }
}
