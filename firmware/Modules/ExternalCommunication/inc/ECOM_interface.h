/*
 * ECOM_interface.h
 *
 *  Created on: Mar 2, 2024
 *      Author: roland
 */

#ifndef MODULES_EXTERNALCOMMUNICATION_INC_ECOM_INTERFACE_H_
#define MODULES_EXTERNALCOMMUNICATION_INC_ECOM_INTERFACE_H_

typedef enum
{
    KeepAlive_e     = 0,
    Command_e       = 1,
    Status_e        = 2
} ECOM_ProtocolHeader_enum;

void ECOM_MainHandler(void);
void ECOM_DataRecievedCallback(void);


#endif /* MODULES_EXTERNALCOMMUNICATION_INC_ECOM_INTERFACE_H_ */
