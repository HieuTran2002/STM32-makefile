#include "stm32g431xx.h"
#include "stm32g431xx_clock.h"


uint32_t GetSysTick(){
    uint8_t usedClockSource = (RCC->CFGR >> 2) & 0b11;

    if      (usedClockSource == 1) { return HSI_FREQ; }
    else if (usedClockSource == 2) { return HSE_FREQ; }
    else if (usedClockSource == 3) {
        
        uint32_t PLL_Source = 0; 
        if (RCC_PLLCFGR_PLLSRC == 2) {
            PLL_Source = HSI_FREQ;
        }
        else if(RCC_PLLCFGR_PLLSRC == 3){
            PLL_Source = HSE_FREQ;
        }
        uint16_t PLLM = (RCC->PLLCFGR >> RCC_PLLCFGR_PLLM_Pos) & 0xF;
        uint16_t PLLN = (RCC->PLLCFGR >> RCC_PLLCFGR_PLLN_Pos) & 0x111F;
        uint16_t PLLP = ((RCC->PLLCFGR >> RCC_PLLCFGR_PLLPDIV_Pos) & 0x1F) + 1;

        return (PLL_Source * (PLLN / PLLM)) /  PLLP;
    }
    return HSI_FREQ;
}

uint32_t GetAHBClock() {
    uint32_t sysClock = GetSysTick();


    // AHB prescaler
    uint8_t ahbPrescaler = (RCC->CFGR >> RCC_CFGR_HPRE_Pos) & 0x0F;
    if (ahbPrescaler < 8) {
        return sysClock;  // No prescaler
    }
    
    return sysClock >> ((ahbPrescaler - 7));  // Apply prescaler
}

uint32_t GetAPB1Clk() {
    uint32_t ahbClock = GetAHBClock();
    
    uint8_t apb1Prescaler = (RCC->CFGR >> RCC_CFGR_PPRE1_Pos) & 0x07;
    if (apb1Prescaler < 4) {
        return ahbClock;  // No prescaler
    }
    
    return ahbClock >> (apb1Prescaler - 3);  // Apply prescaler
}

uint32_t GetAPB2Clk() {
    uint32_t ahbClock = GetAHBClock();

    uint8_t apb2Prescaler = (RCC->CFGR >> RCC_CFGR_PPRE2_Pos) & 0x07;
    if (apb2Prescaler < 4) {
        return ahbClock;  // No prescaler
    }
    
    return ahbClock >> (apb2Prescaler - 3);  // Apply prescaler
}

void delay_ms(uint32_t ms) {
    SysTick->LOAD = (GetSysTick() / 1000) * ms - 1;     // Assuming 16 MHz clock
    SysTick->VAL = 0;                               // Clear the SysTick counter
    SysTick->CTRL = 5;                              // Enable SysTick, no interrupt
    while (!(SysTick->CTRL & (1 << 16)));           // Wait for the COUNTFLAG to be set
    SysTick->CTRL = 0;                              // Disable SysTick
}

