/**
 * @file ByteConversions.c
 *
 * @brief Covert communication bytes to values and vice versa.
 * @details Converting is done in to big endian byte order.
 *
 * =================================================================
 * @author Bc. Roland Molnar
 *
 * =================================================================
 * KEM, FEI, TUKE
 * @date 29.02.2024
 * @defgroup BC Byte conversions
 * @{
 */
#include <ByteConversions.h>

/**
 * @brief Convert 2 bytes to 16 bit integer.
 * @param bytes_p bytes to by converted.
 * @returns union containing 16 bit integers.
 */
Int16Bits_struct BC_2BytesTo16BitData(const U16 * const bytes_p)
{
    Int16Bits_struct ret_s = {0};
    ret_s.val_U16 = *(U16*)bytes_p << (U16)8;                                           /* Write the upper byte. */
    ret_s.val_U16 |= *((U16*)bytes_p + sizeof(U16)) & (U16)0x00FF;                      /* Write the lower byte. */
    return ret_s;
}

/**
 * @brief Convert 4 bytes to 32 bit integer.
 * @param bytes_p bytes to by converted.
 * @returns union containing 32 bit integers.
 */
Int32Bits_struct BC_4BytesTo32BitData(const U16 * const bytes_p)
{
    /* Not worth commenting :D. Just pointer tricks. */
    Int32Bits_struct ret_s = {0};
    ret_s.val_U32 |= (U32)(*((U16*)bytes_p + 2 * sizeof(U16))) << (U32)8;
    ret_s.val_U32 |= (U32)(*((U16*)bytes_p + 3 * sizeof(U16))) << (U32)0;
    ret_s.val_U32 |= (U32)(*((U16*)bytes_p + sizeof(U16))) << (U32)16;
    ret_s.val_U32 |= (U32)(*((U16*)bytes_p)) << (U32)24;

    return ret_s;
}

/**
 * @brief Convert 16 bit data type to bytes.
 * @param val_p pointer to targeted 16 bit variable.
 * @param output_bytes_pU16 is a pointer to array of output bytes
 */
void BC_16BitDataTo2Bytes(const void * const val_p, U16 *restrict output_bytes_pU16)
{
    output_bytes_pU16[1] = *(U16*)val_p & (U16)0x00FF;
    output_bytes_pU16[0] = *(U16*)val_p >> (U16)8;
}

/**
 * @brief Convert 32 bit data type to bytes.
 * @param val_p pointer to targeted 16 bit variable.
 * @param output_bytes_pU16 is a pointer to array of output bytes
 */
void BC_32BitDataTo4Bytes(const void * const val_p, U16 *restrict output_bytes_pU16)
{
    U32 byte_i_U32;
    for(byte_i_U32 = (U32)0; byte_i_U32 < (U32)4; byte_i_U32++)
    {
        output_bytes_pU16[3 - byte_i_U32] = (*(U32*)val_p >> (U32)8 * byte_i_U32) & (U32)0x000000FF;
    }
}

/**
 * @}
 */
