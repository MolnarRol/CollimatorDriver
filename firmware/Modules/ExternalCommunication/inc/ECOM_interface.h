/*
 * ECOM_interface.h
 *
 *  Created on: Mar 2, 2024
 *      Author: roland
 */

#ifndef ECOM_INTERFACE_H_
#define ECOM_INTERFACE_H_

typedef enum
{
    KeepAlive_e     = 0,
    Command_e       = 1,
    Status_e        = 2
} ECOM_ProtocolHeader_enum;

void ECOM_MainHandler(void);

#endif /* ECOM_INTERFACE_H_ */
