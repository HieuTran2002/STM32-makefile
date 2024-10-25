#ifndef STM32F1XX_CLOCK_H
#define STM32F1XX_CLOCK_H

#include "stm32f1xx.h"

void delay_ms(uint32_t ms) {
    SysTick->LOAD = (8000000 / 1000) * ms - 1;
    SysTick->VAL = 0;
    SysTick->CTRL = 5;
    while (!(SysTick->CTRL & (1 << 16)));
    SysTick->CTRL = 0;
}
void delay_micros(uint32_t ms) {
    SysTick->LOAD = (8000000 / 1000000) * ms - 1;
    SysTick->VAL = 0;
    SysTick->CTRL = 5;
    while (!(SysTick->CTRL & (1 << 16)));
    SysTick->CTRL = 0;
}

#endif /* end of include guard: STM32F1XX_CLOCK_H */
