#ifndef STM32F411XX_CLOCK_H
#define STM32F411XX_CLOCK_H

#include "stm32f411xx.h"
#include <stdint.h>

void delay_ms(uint32_t ms) {
    SysTick->LOAD = (8000000 / 1000) * ms - 1;     // Assuming 16 MHz clock
    SysTick->VAL = 0;                               // Clear the SysTick counter
    SysTick->CTRL = 5;                              // Enable SysTick, no interrupt
    while (!(SysTick->CTRL & (1 << 16)));           // Wait for the COUNTFLAG to be set
    SysTick->CTRL = 0;                              // Disable SysTick
}
#endif
