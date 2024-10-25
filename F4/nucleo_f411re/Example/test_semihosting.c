#include "stm32f411xx.h"
#include "stm32f411xx_clock.h"
#include "bsp.h"
#include <stdio.h>

/* Declare semi-hosting */
extern void initialise_monitor_handles(void);

int main(void)
{
    GPIOC_PCLK_EN();
    GPIOA_PCLK_EN();

    GPIOA->MODER |= (1 << 10);
    printf("yeag");

    while (1) {
        if (BSP_BTN_PRESSED) {
            printf("Pressed");
            LED_ON();
        } 
        else{
            LED_OFF();
        }

        delay_ms(10);
    }
    return 1;
}


