/*
 * ll_gpio.h
 *
 *  Created on: Feb 15, 2024
 *      Author: roland
 */

#include <main.h>

#ifndef DRIVERS_LL_GPIO_INC_LL_GPIO_H_
#define DRIVERS_LL_GPIO_INC_LL_GPIO_H_

#define GPIO_BASE                   (Uint32) 0x00007C00
#define GPIOx_MUX_OFFSET            (Uint32) 0x6

typedef struct
{
    Uint16 pin;
    Uint16 pullup_enable;
    Uint16 initial_state;
} LL_LED_struct;

void LL_GPIO_Init(LL_LED_struct* led_config_s);


#endif /* DRIVERS_LL_GPIO_INC_LL_GPIO_H_ */
