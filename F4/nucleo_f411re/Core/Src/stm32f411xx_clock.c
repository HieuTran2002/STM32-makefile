#include "stm32f411xx_clock.h"
#include "stm32f411xx.h"
#include <stdint.h>

void delay_ms(uint32_t ms) {
    SysTick->LOAD = (16000000 / 1000) * ms - 1;
    SysTick->VAL = 0;
    SysTick->CTRL = 5;
    while (!(SysTick->CTRL & (1 << 16)));
    SysTick->CTRL = 0;
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

uint32_t GetAHB_Clock(){
    uint32_t SysClock = GetSysTick();

    uint16_t AHB_PreScale[8] = {2, 4, 8, 16, 64, 128, 256, 512};

    return SysClock / AHB_PreScale[(RCC->CFGR >> 4) & 0x7]; 
}

uint32_t GetAPB1_Clock(){
    uint32_t SysClock = GetSysTick();

    if (!((RCC->CFGR >> 10) & 0b100)) {
        return SysClock;
    }

    uint16_t APB_PreScale[8] = {2, 4, 8, 16};

    uint8_t idx = (RCC->CFGR >> 10) & 0x3;

    idx > 3 ? idx = 3 : 0;

    return SysClock / APB_PreScale[idx]; 
}

uint32_t GetAPB2_Clock(){
    uint32_t SysClock = GetSysTick();

    if (!((RCC->CFGR >> 10) & 0b100)) {
        return SysClock;
    }

    uint16_t APB_PreScale[8] = {2, 4, 8, 16};

    uint8_t idx = (RCC->CFGR >> 13) & 0x3;

    idx > 3 ? idx = 3 : 0;

    return SysClock / APB_PreScale[idx]; 
}
