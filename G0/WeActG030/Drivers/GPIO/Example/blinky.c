#include "stm32g031_gpio_driver.h"
#include "stm32g031xx.h"
#include "stm32g031_clock.h"

void GPIO_Config(void){
    RCC->IOPENR |= 0b111;

    GPIOA->MODER |= 1 << 8;
}

int main(void){
    GPIO_Config();

    while (1) {
        GPIOA->ODR ^= 1 << 4;
        delay_ms(100);
    }
    return 1;
}
