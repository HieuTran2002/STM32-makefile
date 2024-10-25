#include "stm32f411xe.h"
#include <stdint.h>

void GPIO_Init(void) {
    // Enable GPIOB clock
    RCC->AHB1ENR |= (1 << 0);  // Enable GPIOB clock

    // Set GPIOA Pin 5 as output
    GPIOA->MODER |= (1 << (2 * 5));
}

void delay_ms(uint32_t ms) {
    SysTick->LOAD = (16000000 / 1000) * ms - 1;  // Assuming 16 MHz clock
    SysTick->VAL = 0;  // Clear the SysTick counter
    SysTick->CTRL = 5; // Enable SysTick, no interrupt
    while (!(SysTick->CTRL & (1 << 16)));  // Wait for the COUNTFLAG to be set
    SysTick->CTRL = 0;  // Disable SysTick
}

void Clock_Init(void) {
    // Enable HSE (High-Speed External Clock)
    RCC->CR |= (1 << 16);
    
    // Wait until HSE is ready
    while (!(RCC->CR & (1 << 17)));

    // Select HSE as the system clock
    RCC->CFGR |= (1 << 3);
}

int main(void) {
    Clock_Init();   // Initialize the system clock to use HSE
    GPIO_Init();    // Initialize GPIOB for pin 14 output

    while (1) {
        GPIOA->BSRR = (1 << 5);         // Set Pin 14 (turn on LED)
        delay_ms(100);                   // Wait for 100 ms
        GPIOA->BSRR = (1 << (5 + 16));  // Reset Pin 14 (turn off LED)
        delay_ms(100);                   // Wait for 100 ms
    }
}

