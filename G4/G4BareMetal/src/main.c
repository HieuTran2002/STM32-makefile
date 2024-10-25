#include "stm32g431xx.h"
#include <stdint.h>

void delay_ms(uint32_t ms) {
    SysTick->LOAD = (16000000 / 1000) * ms - 1;     // Assuming 16 MHz clock
    SysTick->VAL = 0;                               // Clear the SysTick counter
    SysTick->CTRL = 5;                              // Enable SysTick, no interrupt
    while (!(SysTick->CTRL & (1 << 16)));           // Wait for the COUNTFLAG to be set
    SysTick->CTRL = 0;                              // Disable SysTick
}

int main(void)
{
    RCC->AHB2ENR |= 1;
    GPIOC->MODER &= ~(3 << 12);
    GPIOC->MODER |= (1 << 12);

    while (1) {
        GPIOC->ODR ^= (1 << 6);
        delay_ms(100);
    }
    return 1;
}
