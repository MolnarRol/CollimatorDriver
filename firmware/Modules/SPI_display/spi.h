/**
 * @file spi.h
 * @brief Serial Peripheral Interface, communication standard for LCD display
 * @details Initialization function, function for sending data
 *
 * =================================================================
 * @author Bc. Vadym Holysh
 *
 * =================================================================
 * KEM, FEI, TUKE
 * @date 06.03.2024
 * @addgroup SPI Serial Peripheral Interface
 * @{
 */

#ifndef SPI_H_
#define SPI_H_
#include "F2837xD_device.h"

#define MCU_FREQ (float)200000000 /**< MACRO MCU frequency*/
#define MASTER_SLAVE_MODE 1       /**< MACRO Master-slave mode*/
#define CLOCK_POLARITY 1          /**< MACRO Clock polarity*/
#define CLOCK_PHASE 0             /**< MACRO Clock phase*/


void spi_vInit(float u16BaudRate);

void spi_vSendChar(char cData);

void spi_u16SendData(char *pcData, Uint16 u16Length);

Uint16 spi_u16SendString(char *pcData);

#endif /* SPI_H_ */

/**
 * @}
 */
