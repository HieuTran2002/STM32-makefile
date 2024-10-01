#include "stm32f103xb.h"

void delay_ms(uint32_t ms) {
    SysTick->LOAD = (8000000 / 1000) * ms - 1;
    SysTick->VAL = 0;
    SysTick->CTRL = 5;
    while (!(SysTick->CTRL & (1 << 16)));
    SysTick->CTRL = 0;
}

int main(void){
    /* Enable GPIOA -> D */
    RCC->APB2ENR |= 0xF << 2;

    GPIOB->CRL |= (2 << 8);
    GPIOB->CRL &= ~(1 << 10);

    while(1){
        GPIOB->ODR ^= 1 << 2;
        delay_ms(100);
    }
    return 0;
}
