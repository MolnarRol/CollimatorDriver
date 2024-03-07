/*
 * spi.c
 *
 *  Created on: 29. 2. 2024
 *      Author:
 */
#include "sci.h"
#include "mcu.h"

void SCI_init_void(){
    /*Clock turn on*/
    CpuSysRegs.PCLKCR7.bit.SCI_A=1;

    /*clock divider to 100 MHZ*/
    ClkCfgRegs.LOSPCP.bit.LSPCLKDIV=1;

    /*Transmitter and receiver enabling*/
    SciaRegs.SCICTL1.bit.TXENA=1;
    SciaRegs.SCICTL1.bit.RXENA=1;

    /*Baudrate selection*/
    SciaRegs.SCIHBAUD.bit.BAUD=0b10;   /*650 devided to high and low bits*/
    SciaRegs.SCILBAUD.bit.BAUD=0b10001010;
    SciaRegs.SCICCR.bit.SCICHAR=7;


    /*beztoho nepojde*/
    SciaRegs.SCICTL1.bit.TXWAKE=1;
    //SciaRegs.SCIRXST.bit.RXWAKE=1;

    SciaRegs.SCICTL1.bit.SWRESET=1;
    SciaRegs.SCIPRI.bit.FREESOFT=1;
}

void SCI_SendData_void(char *data, U16 U16_DataLength){
    while(U16_DataLength--){
        /*Control for sending ready*/
        while(!SciaRegs.SCICTL2.bit.TXRDY){

        }
        /*Sending and incrementing pointer*/
        SciaRegs.SCITXBUF.bit.TXDT=*data;
        data++;
    }
}


