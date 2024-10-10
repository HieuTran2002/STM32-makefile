#include "stm32f411xx_clock.h"
#include "stm32f411xx.h"

void delay_ms(uint32_t ms) {
    SysTick->LOAD = (16000000 / 1000) * ms - 1;     // Assuming 16 MHz clock
    SysTick->VAL = 0;                               // Clear the SysTick counter
    SysTick->CTRL = 5;                              // Enable SysTick, no interrupt
    while (!(SysTick->CTRL & (1 << 16)));           // Wait for the COUNTFLAG to be set
    SysTick->CTRL = 0;                              // Disable SysTick
}

uint32_t GetSysTick(){
    uint8_t usedClockSource = (RCC->CFGR >> 2) & 0b11;

    if      (usedClockSource == 1) { return HSI_FREQ; }
    else if (usedClockSource == 2) { return HSE_FREQ; }
    else if (usedClockSource == 3) {
        
        uint32_t PLL_Source = 0; 
        if      (((RCC->PLLCFGR >> 22) & 3) == 0)   { PLL_Source = HSI_FREQ; }
        else if (((RCC->PLLCFGR >> 22) & 3) == 1)   { PLL_Source = HSE_FREQ; }

        uint16_t PLLM = (RCC->PLLCFGR >> 0) & 0xF;
        uint16_t PLLN = (RCC->PLLCFGR >> 6) & 0x111F;
        uint16_t PLLP = ((RCC->PLLCFGR >> 16) & 0x1F) + 1;

        return (PLL_Source * (PLLN / PLLM)) /  PLLP;
    }
    return HSI_FREQ;
}

