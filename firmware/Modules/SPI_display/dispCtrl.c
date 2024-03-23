/*
 * dispCtrl.c
 *
 *  Created on: 10 mar. 2024 ã.
 *      Author: vadym
 */
#include "dispCtrl.h"

 unsigned char reverse(unsigned char b) {
   b = (b & 0xF0) >> 4 | (b & 0x0F) << 4;
   b = (b & 0xCC) >> 2 | (b & 0x33) << 2;
   b = (b & 0xAA) >> 1 | (b & 0x55) << 1;
   return b;
}

void dispCtrl_vSendInstruction(Uint16 u16RW, Uint16 u16RS, char data){

    char packet_3_byte [4];
    char reverse_data = reverse(data);
    packet_3_byte[0] = (0xF8^(u16RW << 2)^(u16RS << 1));
    packet_3_byte[1] = (reverse_data & 0xF0);
    packet_3_byte[2] = (reverse_data & 0x0F) << 4;

    spi_u16SendData(packet_3_byte,3);
}

void dispCtrl_vSendInitInstruction(char data){

    char packet_3_byte [3];
    char reverse_data = reverse(data);
    packet_3_byte[0] = (reverse_data & 0xF0);
    packet_3_byte[1] = (reverse_data & 0x0F) << 4;

    spi_u16SendData(packet_3_byte,2);
}


void dispCtrl_vInitDisplay(void){

    DELAY_US(7000);
    /* set RESET pin for display*/

    GpioCtrlRegs.GPAGMUX2.bit.GPIO30 = 1;
    GpioCtrlRegs.GPAGMUX2.bit.GPIO30 = 0;
    GpioCtrlRegs.GPADIR.bit.GPIO30 = 1;
    GpioDataRegs.GPACLEAR.bit.GPIO30 = 1;
    GpioDataRegs.GPASET.bit.GPIO30 = 1;

    DELAY_US(12000);
    GpioDataRegs.GPACLEAR.bit.GPIO30 = 1;
    DELAY_US(2000);

    //spi_vSendChar( (0xF8 ^ ( (char)0 << 2 ) ^ ( (char)0 << 1) ) ); /*start byte - 5 high,RW,RS*/


    dispCtrl_vSendInstruction(0,0,(char)0x3A); /*8 bit length*/
    DELAY_US(1000);
//    dispCtrl_vSendInstruction(0,0,(char)0x3F); /*normal display*/
//    DELAY_US(1000);
    dispCtrl_vSendInstruction(0,0,(char)0x09); /*4 line display 0x09*/
    DELAY_US(1000);
    dispCtrl_vSendInstruction(0,0,(char)0x06); /*bottom view*/
    DELAY_US(1000);
    dispCtrl_vSendInstruction(0,0,(char)0x1E); /*Bias setting BS = 1*/
    DELAY_US(1000);
    dispCtrl_vSendInstruction(0,0,(char)0x39); /*set instruction*/
    DELAY_US(1000);
    dispCtrl_vSendInstruction(0,0,(char)0x1B); /*internal OSC*/
    DELAY_US(1000);
    dispCtrl_vSendInstruction(0,0,(char)0x6C); /*divider on*/
    DELAY_US(1000);
    dispCtrl_vSendInstruction(0,0,(char)0x54); /*booster on 56 */
    DELAY_US(1000);
    dispCtrl_vSendInstruction(0,0,(char)0x72); /*set contrast 7A*/
    DELAY_US(1000);
    dispCtrl_vSendInstruction(0,0,(char)0x38); /*set instruction*/
    DELAY_US(1000);
    dispCtrl_vSendInstruction(0,0,(char)0x0F); /*display on, cursor on, blink on*/
    DELAY_US(1000);
    dispCtrl_vSendInstruction(0,0,(char)0x01); /*clear display*/
    DELAY_US(1000);
    dispCtrl_vSendInstruction(0,0,(char)0x80); /*set DDRAM on position 0:0*/
    DELAY_US(1000);

    DELAY_US(5000);

}

void dispCtrl_u16PutString(char* pcData){

    Uint16 i = 0;
    spi_vSendChar( (0xF8 ^ ( (char)0 << 2 ) ^ ( (char)1 << 1) ) ); /*start byte - 5 high,RW,RS*/
    while(pcData[i] != '\0')
    {
        spi_vSendChar(reverse(pcData[i]) & 0xF0);
        spi_vSendChar((reverse(pcData[i]) & 0x0F) << 4);
        i++;
    }
}

void dispCtrl_vSetPosition(Uint16 u16PosX, Uint16 u16PosY){
    char cAddress = (char)( ( ( (Uint16)u16PosY - 1 )*(Uint16)32 ) + ( (Uint16)u16PosX - 1 ) ) ;
    dispCtrl_vSendInstruction(0,0,((char)0x80 + (char)cAddress)); /*set DDRAM on position 0:0*/
    DELAY_US(1000);
}


