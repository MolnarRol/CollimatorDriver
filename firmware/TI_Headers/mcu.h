/**
 * \file mcu.h
 * \brief Contains functions for basic microcontroller settings
 *
 * Module 'mcu' serves for basic MCU settings (e.g. clocks or flash)
 * and contains macros for MCU clock frequency - MCU_CPUFREQ_MHZ -
 * and macro for simple delay function by TI - DELAY_US(A).
 *
 * \author Viktor Slapak
 * \date 2022
 * \version 0.1
 * \copyright Only for education and research purposes of
 * Department of Electrical Engineering and Mechatronics
 * at Technical University of Kosice, Slovakia
 */

#ifndef MCU_H_
#define MCU_H_

#include <F2837xD_device.h>

#define MCU_XTAL_FREQ_MHZ 20L //!< Crystal frequency in MHz
#define MCU_XTALOSC 1 //!< Selects system oscillator to crystal

#define MCU_MULINT  20 //!< Sets multiplier for PLL to 20x.
#define MCU_MULFAC  0 //!< Sets fractional multiplier for PLL to 0x.
#define MCU_PLLDIV  1 //!< Sets PLL divider to /2.

#define MCU_LOSPCLKDIV 7 //!< Sets the low speed clock divider to 7 (7 is maximum)

// working frequency is set to f_OSC * MCU_MULINT/(2*MCU_PLLDIV),
// if MCU_PLLDIV = 0, the working frequency is f_OSC * MCU_MULINT


#if MCU_PLLDIV != 0
    #define MCU_CPUFREQ_MHZ (MCU_XTAL_FREQ_MHZ*(MCU_MULINT+MCU_MULFAC)/(2*MCU_PLLDIV)) //!< Selected CPU frequency
#else
    #define MCU_CPUFREQ_MHZ (MCU_XTAL_FREQ_MHZ*(MCU_MULINT+MCU_MULFAC)) //!< Selected CPU frequency
#endif


/********** helper macro for delay *******/
#define CPU_RATE 5L // for 200 MHz device is one step duration = 5 ns
extern void F28x_usDelay(long LoopCount);
// DO NOT MODIFY THIS LINE.
#define DELAY_US(A) F28x_usDelay(((((long double) A * 1000.0L) / (long double)CPU_RATE) - 9.0L) / 5.0L)


/***************PROTOTYPES******************/

/**
 * \brief Function for disabling watchdog.
 */
void mcu_vDisableDog(void);

/**
 * \brief Initializes microcontroller clocks.
 *
 *
 * The final MCU clock frequency is computed as f_CPU=f_XTAL*(MulInt+MulFrac)/PllDiv .
 * The parameters MulInt, MulFrac and PllDiv are defined by macros.
 *
 * \note Function is based on: \n
 * \a F2837xD_SysCtrl.c - F2837xD Device System Control Initialization & Support Functions. \n
 * Texas Instruments F2837xD Support Library v210 from Tue Nov  1 14:46:15 CDT 2016 \n
 * Copyright (C) 2013-2016 Texas Instruments Incorporated
 */
void mcu_vInitClocks(void);

// initialization of FLASH memory
// parameters: none
void mcu_vInitFlash(void);

/**
 * \brief Initializes PWM modules and appropriate GPIOs
 *
 * Function initializes the ePWM3 and ePWM6 modules and GPIOs according to
 * electrical interconnections in CBIEV1 with 50AV3.
 *
 * The bridge control pins are: \n
 * (signal - connector pin - GPIO) \n
 * SD1 - X2.6 - GPIO2 \n
 * SD2 - X2.11 - GPIO12 \n
 * SD3 - X2.7 - GPIO3 \n
 * PWM1 - X2.8 - GPIO4 - EPWM3A \n
 * PWM2 - X2.10 - GPIO11 - EPWM6B \n
 * PWM3 - X2.9 - GPIO10 - EPWM6A
 *
 * DCBus voltage sensing is on pin X2.2 - ADCINB1
 *
 * \param u16pwmFrequency_Hz Requested frequency of PWM output in Hz.
 * \note The frequency can be set from 750 Hz higher.
 *
 * \warning This function will not start PWM ! To start the PWM, use \a mcu_vEnablePWM() .
 */
void mcu_vInitPWM(Uint16 u16pwmFrequency_Hz);

/**
 * \brief Enables PWM counters
 */
void mcu_vEnablePWM(void);

/**
 * \brief Disables and resets PWM counters
 */
void mcu_vDisablePWM(void);

/**
 * \brief Initializes ADC module
 *
 * Initializes the ADC according to the modular servodrive pinout
 *
 * \return Returns 1 when finnished.
 */
Uint16 mcu_u16InitAdc(void);

/**
 * \brief Initializes QEP module
 *
 * Initializes the QEP according to the modular servodrive pinout
 *
 * \param u32Period_us Internal QEP timer period in microseconds
 * \param u16NoOfIncrements Number of encoder pulses per revolution
 * \param u16SwapInputs If not zero, the tracks A and B are swapped
 *
 * \return Returns unit time out period register value QUPRD.
 */
Uint32 mcu_initQep (Uint32 u32Period_us, Uint16 u16NoOfIncrements, Uint16 u16SwapInputs);

/**
 * \brief Initializes Timer0
 *
 * Initializes Timer0 and enables interrupt in Timer0 module.
 *
 * \param u32Period_us Timer period in microseconds
 * \return Returns PRD value
 */
Uint32 mcu_u32InitTimer0(Uint32 u32Period_us);

/**
 * \brief Initializes SCI module on GPIO64 & 65
 *
 * Initializes the SCI-A module with the 50 Mhz clock, 8 bit character,
 * 1 stop bit, no parity.
 *
 *
 * \param u16Baudrate is the requested baudrate of the SCI
 * \param u16EnableFIFO enables FIFO if sets to 1, otherwise the FIFO is disabled
 */
Uint32 mcu_u32InitSci(Uint32 u32Baudrate, Uint16 u16EnableFIFO);

/**
 * \brief Initializes SPI module on GPIO58, GPIO60 & GPIO61
 *
 * Initializes the SPI-B module to Mode 3. GPIO59 is set to output to manual control the CS pin.
 * Use of the FIFO is enabled. No interrupts.
 *
 *
 * \param u16BaudRate_kHz is the requested baudrate of the SPI
 * \param u16CharLength sets the character length, min. 1 max. 16 bit
 * \return Returns calculated BRR
 */
void mcu_vInitSpi(Uint16 u16BaudRate_kHz, Uint16 u16CharLength);

#endif /* MCU_H_ */
