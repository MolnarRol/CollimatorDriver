/*
 * DEBUG.c
 *
 *  Created on: Mar 4, 2024
 *      Author: roland
 */
#include <DEBUG.h>

void DEBUG_Init(void)
{
    EALLOW;
    GpioCtrlRegs.GPADIR.bit.GPIO31 = 1;
    GpioCtrlRegs.GPBDIR.bit.GPIO34 = 1;
    EDIS;

    GpioDataRegs.GPASET.bit.GPIO31 = 1;
    GpioDataRegs.GPBSET.bit.GPIO34 = 1;
}


