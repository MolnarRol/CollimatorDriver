/**
 * @file spi.c
 * @brief Serial Peripheral Interface, communication standard for LCD display
 * @details Initialization function, function for sending data
 *
 * =================================================================
 * @author Bc. Vadym Holysh
 *
 * =================================================================
 * KEM, FEI, TUKE
 * @date 06.03.2024
 * @defgroup SPI Serial Peripheral Interface
 * @{
 */

#include "spi.h"

/**
 * @brief Initialization SPI module with FIFO.
 * @details 600 kHz < Baudrate < 920 kHz
 * @param Reference Baudrate.
 */

void spi_vInit(float u16BaudRate){

    EALLOW;
    ClkCfgRegs.LOSPCP.bit.LSPCLKDIV = 1; /* set LSPCLK divider on =/2 - 50*/
    CpuSysRegs.PCLKCR8.bit.SPI_B = 1;

    GpioCtrlRegs.GPBGMUX2.bit.GPIO58 = 1;
    GpioCtrlRegs.GPBMUX2.bit.GPIO58 = 2;

    GpioCtrlRegs.GPBGMUX2.bit.GPIO59 = 1;
    GpioCtrlRegs.GPBMUX2.bit.GPIO59 = 2;
    //GpioCtrlRegs.GPBDIR.bit.GPIO59 = 1;


    GpioCtrlRegs.GPBGMUX2.bit.GPIO60 = 1;
    GpioCtrlRegs.GPBMUX2.bit.GPIO60 = 2;

    GpioCtrlRegs.GPBGMUX2.bit.GPIO61 = 1;
    GpioCtrlRegs.GPBMUX2.bit.GPIO61 = 2;
   // GpioCtrlRegs.GPBQSEL2.bit.GPIO61 = 3;


    EDIS;
    SpibRegs.SPICCR.bit.SPISWRESET = 0;

    #if CLOCK_POLARITY == 1
        SpibRegs.SPICCR.bit.CLKPOLARITY = 1;  /*sampling falling edge*/
    #else
        SpibRegs.SPICCR.bit.CLKPOLARITY = 0;  /*sampling raising edge*/
    #endif

    #if CLOCK_PHASE == 1
        SpibRegs.SPICTL.bit.CLK_PHASE = 1;   /*with delay*/
    #else
        SpibRegs.SPICTL.bit.CLK_PHASE = 0;   /*without delay*/
    #endif


    #if MASTER_SLAVE_MODE == 1
        SpibRegs.SPICTL.bit.MASTER_SLAVE = 1;      /*master-mode*/
    #else
        SpibRegs.SPICTL.bit.MASTER_SLAVE = 0;      /*slave-mode*/
    #endif

    SpibRegs.SPICCR.bit.SPICHAR = 7;           /*8 bit*/
    SpibRegs.SPICCR.bit.SPILBK = 1 ;           /*loop-back mode*/
    SpibRegs.SPICTL.bit.TALK = 1;              /*enable transmit data*/
    SpibRegs.SPIBRR.bit.SPI_BIT_RATE =
    (Uint16)(((float)MCU_FREQ/2.0) / (u16BaudRate)) - 1;     /*bit rate*/
    SpibRegs.SPIPRI.bit.FREE = 1;              /*free mode debug*/
    SpibRegs.SPISTS.bit.INT_FLAG = 1;
    SpibRegs.SPISTS.bit.OVERRUN_FLAG = 1;

    /* Initialization FIFO*/

    SpibRegs.SPIFFTX.bit.SPIFFENA = 1;          // SPI FIFO enhancements are enabled
    SpibRegs.SPIFFTX.bit.TXFFINTCLR = 1;        // clear SPIFFTX
    SpibRegs.SPIFFRX.bit.RXFFOVFCLR = 1;        // Receive FIFO overflow clear
    SpibRegs.SPIFFRX.bit.RXFFINTCLR = 1;        // Receive FIFO Interrupt Clear
    SpibRegs.SPIFFTX.bit.TXFIFO = 1;            // release transmit FIFO from reset
    SpibRegs.SPIFFRX.bit.RXFIFORESET = 1;       // Re-enable receive FIFO operation
    SpibRegs.SPIPRI.bit.FREE = 1;               // Emulation Free Run regardless of suspend
    SpibRegs.SPIFFTX.bit.SPIRST = 1;            // SPI FIFO can resume transmit or receive


    SpibRegs.SPICCR.bit.SPISWRESET = 1;
    }

/**
 * @brief Send data - one char using FIFO.
 * @param Data.
 */
void spi_vSendChar(char cData){

//    while (SpibRegs.SPISTS.bit.BUFFULL_FLAG == 1); //Overenie ci je buffer rdy
//           SpibRegs.SPITXBUF = cData;

    //while(SpibRegs.SPISTS.bit.INT_FLAG != 0);
    //do{}while(SpibRegs.SPISTS.bit.BUFFULL_FLAG == 1);
    while(SpibRegs.SPIFFTX.bit.TXFFST != 0);
    SpibRegs.SPITXBUF = (cData & 0xFF) << 8;

}

/**
 * @brief Send data - one string using FIFO.
 * @param Address to char array.
 * @param Length array
 */
void spi_u16SendData(char *pcData, Uint16 u16Length){

//    int counter;
//    for (counter = 0; counter < u16Length; counter++)
//    {
//        spi_vSendChar(pcData[counter]);
//    }

    while(u16Length--){
        //while(SpibRegs.SPISTS.bit.BUFFULL_FLAG == 1);
        while(SpibRegs.SPIFFTX.bit.TXFFST != 0);
        SpibRegs.SPITXBUF = (*(pcData++))<<(8);
    }
}

/**
 * @brief Send data - one string using FIFO.
 * @param Address to char array.
 * @return Length char array.
 */
Uint16 spi_u16SendString(char *pcData){

    Uint16 i;

    for(i = 0; pcData[i] != '\0'; i++){
        spi_vSendChar(pcData[i]);
    }

    return (i);
}

/**
 * @}
 */



