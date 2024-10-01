#ifndef STM32G031_CLOCK_H
#define STM32G031_CLOCK_H

#include "stm32g031xx.h"

void delay_ms(uint32_t ms) {
    SysTick->LOAD = (16000000 / 1000) * ms - 1;      // Assuming 16 MHz clock
    SysTick->VAL = 0;                               // Clear the SysTick counter
    SysTick->CTRL = 5;                              // Enable SysTick, no interrupt
    while (!(SysTick->CTRL & (1 << 16)));           // Wait for the COUNTFLAG to be set
    SysTick->CTRL = 0;                              // Disable SysTick
}

#endif /* end of include guard: STM32G031_CLOCK_H */

