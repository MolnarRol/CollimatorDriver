/*
 * SCI.h
 *
 *  Created on: Feb 29, 2024
 *      Author: roland
 */
#include <main.h>

#ifndef DRIVERS_SCI_DRIVER_INC_SCI_H_
#define DRIVERS_SCI_DRIVER_INC_SCI_H_

#define SCI_RX_BYTES_AVAILABLE_dM           ( SciaRegs.SCIFFRX.bit.RXFFST )
#define SCI_TX_BYTES_AVAILABLE_dm           ( (U16)16 - SciaRegs.SCIFFTX.bit.TXFFST )
#define SCI_RX_BUFF_dM                      ( SciaRegs.SCIRXBUF.all )
#define SCI_TX_BUFF_dM                      ( SciaRegs.SCITXBUF.all )

void SCI_Init(void);
void SCI_SendData(const U16 * const data_pU16, const U16 n_data_U16);



#endif /* DRIVERS_SCI_DRIVER_INC_SCI_H_ */
