#include "stm32f411xx.h"
#include "stm32f411xx_gpio_driver.h"
#include <stdint.h>


void GPIOs_Init(void) {
    // Enable GPIOB clock
    
    GPIOA_PCLK_EN();
    GPIOB_PCLK_EN();
    GPIOC_PCLK_EN();

    GPIO_Handle_Type GPIO_Input;

    GPIO_Input.pGPIO                            = GPIOB;
    GPIO_Input.GPIO_Config.PinNumber            = 15;
    GPIO_Input.GPIO_Config.PinMode              = GPIO_MODER_OUTPUT;
    GPIO_Init(&GPIO_Input);

    GPIO_Input.pGPIO                            = GPIOB;
    GPIO_Input.GPIO_Config.PinNumber            = 12;
    GPIO_Input.GPIO_Config.PinMode              = GPIO_MODER_OUTPUT;
    GPIO_Init(&GPIO_Input);

    GPIO_Input.pGPIO                            = GPIOB;
    GPIO_Input.GPIO_Config.PinNumber            = 13;
    GPIO_Input.GPIO_Config.PinMode              = GPIO_MODER_OUTPUT;
    GPIO_Init(&GPIO_Input);

    GPIO_Input.pGPIO                            = GPIOA;
    GPIO_Input.GPIO_Config.PinNumber            = 5;
    GPIO_Input.GPIO_Config.PinMode              = GPIO_MODER_OUTPUT;
    GPIO_Init(&GPIO_Input);
}

void delay_ms(uint32_t ms) {
    SysTick->LOAD = (16000000 / 1000) * ms - 1;     // Assuming 16 MHz clock
    SysTick->VAL = 0;                               // Clear the SysTick counter
    SysTick->CTRL = 5;                              // Enable SysTick, no interrupt
    while (!(SysTick->CTRL & (1 << 16)));           // Wait for the COUNTFLAG to be set
    SysTick->CTRL = 0;                              // Disable SysTick
}

void Clock_Init(void) {
    RCC->CR |= (1 << 16);
    
    while (!(RCC->CR & (1 << 17)));

    RCC->CFGR |= (1 << 0x1);
}

int main(void) {
    Clock_Init();   // Initialize the system clock to use HSE
    GPIOs_Init();    // Initialize GPIOB for pin 14 output

    while (1) {
    GPIO_TogglePin(GPIOA, 5);
        delay_ms(100);
    }
}

