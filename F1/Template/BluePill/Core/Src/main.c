#include "stm32f103xb.h"
#include <stdint.h>


void clock_init(void){
    /* Reset CR */
    RCC->CR &= ~1U;

    /* Enable HSI */
    RCC->CR |= 1;

    /* Wait for HSI */
    while (!(RCC->CR & (1 << 1)));
}

void gpio_init(void){
    /* Enable GPIOA->D */
    RCC->APB2ENR |= 0xF << 2;

    /* Clear CRL */
    GPIOB->CRL = 0;

    /* Output: PB2, PB5 */
    GPIOB->CRL |= 1 << 8 | 1 << 20;
}

void delay_ms(uint32_t ms) {
    SysTick->LOAD = (8000000 / 1000) * ms - 1;
    SysTick->VAL = 0;
    SysTick->CTRL = 5;
    while (!(SysTick->CTRL & (1 << 16)));
    SysTick->CTRL = 0;
}

int main(void)
{
    clock_init();
    gpio_init();
    while (1) {
        GPIOB->ODR ^= 1 << 2 | 1 << 5;
        delay_ms(100);
    }
    return 0;
}
