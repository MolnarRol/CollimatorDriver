/*
 * spi.h
 *
 *  Created on: 6 mar. 2024 ã.
 *      Author: vadym
 */

#ifndef SPI_H_
#define SPI_H_
#include "F2837xD_device.h"

#define MCU_FREQ (float)200000000 // MCU frequency
#define MASTER_SLAVE_MODE 1
#define CLOCK_POLARITY 1
#define CLOCK_PHASE 0



unsigned char reverse(unsigned char b);

void spi_vInit(float u16BaudRate);

void spi_vSendChar(char cData);

void spi_u16SendData(char *pcData, Uint16 u16Length);

Uint16 spi_u16SendString(char *pcData);

#endif /* SPI_H_ */
