/*
 * ll_gpio.c
 *
 *  Created on: Feb 15, 2024
 *      Author: roland
 */
#include <ll_gpio.h>

void LL_GPIO_Init(LL_LED_struct* led_config_s)
{
//    Uint16 gpio_port = led_config_s->pin / (Uint16)32;
//    Uint16 gpio_pin_in_port = led_config_s->pin % (Uint16)32;
//    Uint16 second_half = gpio_pin_in_port / 16;
//
//    EALLOW;
//    /* Mux setting */
//    * (Uint16*)(GPIO_BASE + GPIOx_MUX_OFFSET + (2 * second_half)) |= (1 << 2*(gpio_pin_in_port - 16 * second_half));
//
//    EDIS;
}



void gpio_vSetLed(LED_NUM led_num, Uint16 state)
{
    switch(led_num)
    {
        case LED2:
            if(state == 0)
            {
                //clear
            }
            else
            {
                //set
            }
            break;

        case LED3:
            if(state == 0)
            {
                //clear
            }
            else
            {
                //set
            }
            break;

        default: break;
    }

}
