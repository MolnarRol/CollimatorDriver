/*
 * SCI.h
 *
 *  Created on: Feb 29, 2024
 *      Author: roland
 */
#include <main.h>

#ifndef DRIVERS_SCI_DRIVER_INC_SCI_H_
#define DRIVERS_SCI_DRIVER_INC_SCI_H_

void SCI_Init(void);
void SCI_SendData(const U16 const *data_pU16, const U16 n_data_U16);



#endif /* DRIVERS_SCI_DRIVER_INC_SCI_H_ */
