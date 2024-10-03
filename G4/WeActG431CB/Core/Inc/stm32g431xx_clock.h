#ifndef STM32G431XX_CLOCK_H
#define STM32G431XX_CLOCK_H

#include "stm32g431xx.h"
#include <stdint.h>

#define HSE_FREQ    8000000
#define HSI_FREQ    16000000

void delay_ms(uint32_t ms);

uint32_t GetSysTick();

uint32_t GetAHBClock();

uint32_t GetAPB1Clk();
uint32_t GetAPB2Clk();

#endif
