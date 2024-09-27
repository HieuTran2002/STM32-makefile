#include "stm32f411xx.h"

int main(void)
{
    RCC->AHB1ENR |= 1;
    GPIOA->MODER |= 1 << 10;
    
    while (1) {
        GPIOA->ODR ^= 1 << 5;
        for (int i=0; i<160000; i++);
    }

    return 0;
}
