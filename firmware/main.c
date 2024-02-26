#include "mcu.h"
#include "ll_gpio.h"

/**
 * main.c
 */
int main(void)
{
    mcu_vInitClocks();

    LL_LED_struct led_conf_s;

     led_conf_s.pin = 31;
     led_conf_s.initial_state = 0;
     led_conf_s.pullup_enable = 0;

    LL_GPIO_Init(&led_conf_s);

    while(1)
    {
        GpioDataRegs.GPASET.bit.GPIO31 = 1;
        DELAY_US(5E5);
        GpioDataRegs.GPACLEAR.bit.GPIO31 = 1;
        DELAY_US(5E5);
    }
}
