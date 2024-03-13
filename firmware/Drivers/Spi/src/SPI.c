/*
 * SPI.c
 *
 *  Created on: Mar 7, 2024
 *      Author: roland
 */
#include <SPI.h>

void SPI_Init(void)
{
    EALLOW;
    CpuSysRegs.PCLKCR8.bit.SPI_A = 1;                   /* Enable SPI module clock. */

    /* GPIO setup for SPI interface. */
    /* MOSI pin - GPIO54 */
    GpioCtrlRegs.GPBGMUX2.bit.GPIO54 = 0;
    GpioCtrlRegs.GPBMUX2.bit.GPIO54 = 1;

    /* MISO pin - GPIO55 */
    GpioCtrlRegs.GPBGMUX2.bit.GPIO55 = 0;
    GpioCtrlRegs.GPBMUX2.bit.GPIO55 = 1;

    /* CLK pin - GPIO56 */
    GpioCtrlRegs.GPBGMUX2.bit.GPIO56 = 0;
    GpioCtrlRegs.GPBMUX2.bit.GPIO56 = 1;

    /* CS pin - GPIO57 */
    GpioCtrlRegs.GPBGMUX2.bit.GPIO57 = 0;
    GpioCtrlRegs.GPBMUX2.bit.GPIO57 = 1;


    /* SPI setup. */
    SpiaRegs.SPICCR.bit.SPISWRESET = 0;
    SpiaRegs.SPICCR.bit.SPICHAR =  7;                   /* 8-bit words */
    SpiaRegs.SPICTL.bit.MASTER_SLAVE = 1;

    /* SPI mode setup. */
    SpiaRegs.SPICCR.bit.CLKPOLARITY = 0;                /* TODO: Polarity. */
    SpiaRegs.SPICTL.bit.CLK_PHASE = 0;                  /* TODO: Phase */

    /* Baud rate setup. */
    SpiaRegs.SPIBRR.bit.SPI_BIT_RATE = 0;               /* TODO: Calculate bit rate. */


    SpiaRegs.SPICCR.bit.SPISWRESET = 1;                 /*  */
    EDIS;

}
