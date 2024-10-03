#include "stm32f411xx.h"
#include "stm32f411xx_clock.h"

void GPIO_Config(void){
    GPIOA_PCLK_EN();
    GPIOA->MODER |= 1 << 10;
}

int main(void)
{
    GPIO_Config();
    while (1) {
        GPIOA->ODR ^= 1 << 5;
        delay_ms(100);
    }
    return 1;
}
