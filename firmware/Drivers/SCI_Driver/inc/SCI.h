/*
 * SCI.h
 *
 *  Created on: Feb 29, 2024
 *      Author: roland
 */
#include <main.h>

#ifndef DRIVERS_SCI_DRIVER_INC_SCI_H_
#define DRIVERS_SCI_DRIVER_INC_SCI_H_

#define SCI_RX_BYTES_AVAILABLE_dMU16        ( SciaRegs.SCIFFRX.bit.RXFFST )
#define SCI_TX_BYTES_AVAILABLE_dMU16        ( (U16)16 - SciaRegs.SCIFFTX.bit.TXFFST )
#define SCI_TX_BUFFER_EMPTY_dM              ( SciaRegs.SCIFFTX.bit.TXFFST == (U16)0 )
#define SCI_RX_BUFF_dM                      ( SciaRegs.SCIRXBUF.all )
#define SCI_TX_BUFF_dM                      ( SciaRegs.SCITXBUF.all )

void SCI_Init(void);
void SCI_SendData(const U16 * const data_pU16, const U16 n_data_U16);
void SCI_SetRxEnableState(boolean enabled_b);
void SCI_SetTxEnableState(boolean enabled_b);


#endif /* DRIVERS_SCI_DRIVER_INC_SCI_H_ */
