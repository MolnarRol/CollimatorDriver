/**
 * @file main.c
 * @brief Main application file
 * @details Details
 *
 * =================================================================
 * @author Bc. Samuel Fertal
 * @author Bc. Vadym Holysh
 * @author Bc. Roland Molnar
 *
 * =================================================================
 * KEM, FEI, TUKE
 * @date 27.02.2024
 */
#include <main.h>
#include <ATB_interface.h>
#include <AC_interface.h>
#include <ECOM_interface.h>
#include <MDA_interface.h>
#include <PWM_interface.h>
#include <SCI.h>
#include <TEST.h>
#include <ByteConversions.h>
#include <CRC_interface.h>

U16 data_aU16[4] = {0x11, 0x20, 0x30, 0x81};
U16 crc;
boolean output_en = False_b;

/**
 * @brief Main function
 */
void main(void)
{
    /* Initialization */
    mcu_vInitClocks();                                          /* Initialize uC clock system. */
    ATB_Init();
    MDA_Init();
    PWM_Init();
    SCI_Init();
    TEST_PinInit();
    /* Main loop */
    while(1)
    {
        crc = CRC8_CCITT_U16(data_aU16, 4);
        PWM_SetOutputEnable(output_en);
        ECOM_MainHandler();
//        AC_MainHandler();                                       /* Application control main handler. */
//        ECOM_TxRequest(data_aU16, 12);
    }
}

/**
 * @brief Error handler
 */
#pragma RETAIN ( ErrorHandler )
void ErrorHandler(void)
{
    DINT;                                                       /* Disable interrupts. */
    for(;;)                                                     /* Endless loop. */
    {
        // Do nothing.
    }
}
