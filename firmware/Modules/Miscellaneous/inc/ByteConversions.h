/*
 * ByteConversions.h
 *
 *  Created on: Apr 12, 2024
 *      Author: roland
 */

#ifndef MODULES_MISCELLANEOUS_INC_BYTECONVERSIONS_H_
#define MODULES_MISCELLANEOUS_INC_BYTECONVERSIONS_H_
#include <app_types.h>

typedef union
{
    U16 val_U16;
    S16 val_S16;
} Int16Bits_struct;

typedef union
{
    U32 val_U32;
    S32 val_S32;
} Int32Bits_struct;

Int16Bits_struct BC_2BytesTo16BitData(const void *restrict const bytes_p);
Int32Bits_struct BC_4BytesTo32BitData(const void *restrict const bytes_p);
void BC_16BitDataTo2Bytes(const void *restrict const val_U16, U16 *restrict output_bytes_pU16);
void BC_32BitDataTo4Bytes(const void *restrict const val_U32, U16 *restrict output_bytes_pU16);


#endif /* MODULES_MISCELLANEOUS_INC_BYTECONVERSIONS_H_ */
