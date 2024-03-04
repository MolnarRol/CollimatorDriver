/*
 * CircularBuffer.h
 *
 *  Created on: Mar 4, 2024
 *      Author: roland
 */

#include <app_types.h>

#ifndef CIRCULARBUFFER_H_
#define CIRCULARBUFFER_H_

typedef struct
{
    const U16       buffer_size_U16;
} CB_Handle_struct;

void CB_Init(CB_Handle_struct* cb_ps, const buffer_size_);



#endif /* CIRCULARBUFFER_H_ */
