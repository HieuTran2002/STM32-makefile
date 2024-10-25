#include "stdint.h"
#include "stm32g431xx.h"
#include "stm32g431_gpio_drivers.h"
#include "stm32g431xx_clock.h"

void GPIO_Config(void){
    GPIOC_PCLK_EN();
    GPIOC->MODER &= ~(3 << 12);
    GPIOC->MODER |= 1 << 12;
}
int main(void){
    GPIO_Config();
    while (1) {
        GPIOC->ODR ^=  1 << 6;
        delay_ms(100);
    }
    return 1;
}
