/*
 * gpio.c
 *
 *  Created on: 29. 2. 2024
 *      Author: samof
 */
#include "mcu.h"
#include "gpio.h"


/* SCI communication GPIO initialization*/

void GPIO_init_void(){

    EALLOW;

    /*Selecting gpio for SCI TX and RX*/

    GpioCtrlRegs.GPAGMUX2.bit.GPIO28=0;
    GpioCtrlRegs.GPAMUX2.bit.GPIO28=1;

    GpioCtrlRegs.GPAGMUX2.bit.GPIO29=0;
    GpioCtrlRegs.GPAMUX2.bit.GPIO29=1;


}
