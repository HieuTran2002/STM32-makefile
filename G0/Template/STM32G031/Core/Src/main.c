#include "stm32g031xx.h"
#include <stdint.h>

void delay_ms(uint32_t ms) {
    SysTick->LOAD = (16000000 / 1000) * ms - 1;      // Assuming 16 MHz clock
    SysTick->VAL = 0;                               // Clear the SysTick counter
    SysTick->CTRL = 5;                              // Enable SysTick, no interrupt
    while (!(SysTick->CTRL & (1 << 16)));           // Wait for the COUNTFLAG to be set
    SysTick->CTRL = 0;                              // Disable SysTick
}

int main(void)
{
    RCC->CR = 0;
    RCC->CR |= 1 << 8;
    /* Enable GPIOA */
    RCC->IOPENR |= 0b1;


    /* Reset GPIO */
    GPIOA->MODER = 0;
    /* Config PA4 */
    GPIOA->MODER  |= 1 << 8;

    while (1) {
        GPIOA->ODR ^= 1 << 4;
        delay_ms(100);
    }
}
