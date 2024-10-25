#ifndef STM32F411XX_CLOCK_H
#define STM32F411XX_CLOCK_H

#include <stdint.h>

#define HSE_FREQ    8000000
#define HSI_FREQ    16000000

void delay_ms(uint32_t ms);

uint32_t GetSysTick();

#endif
