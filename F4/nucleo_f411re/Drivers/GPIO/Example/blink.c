#include "stm32f411xx.h"
#include "stm32f411xx_clock.h"
int main(void)
{
    RCC->AHB1ENR |= 1;
    GPIOA->MODER |= 1 << 10;
    
    while (1) {
        GPIOA->ODR ^= 1 << 5;
        delay_ms(50);
    }

    return 0;
}
