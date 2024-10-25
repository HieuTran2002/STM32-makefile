#include "stm32f411xx.h"
#include "stm32f411xx_gpio_driver.h"
#include "stm32f411xx_spi_driver.h"
#include <stdint.h>


void GPIOs_Init(void) {
    // Enable GPIOB clock
    
    GPIOA_PCLK_EN();
    GPIOB_PCLK_EN();
    GPIOC_PCLK_EN();

    GPIO_Handle_Type GPIO_SPI2;

    GPIO_SPI2.pGPIO                 = GPIOB;
    GPIO_SPI2.GPIO_Config.PinNumber = 10;  
    GPIO_SPI2.GPIO_Config.PinMode   = GPIO_MODER_ALTFNC;
    GPIO_SPI2.GPIO_Config.Pull      = GPIO_PUPDR_NONE;
    GPIO_SPI2.GPIO_Config.Speed     = GPIO_OSPEED_HIGH;
    GPIO_SPI2.GPIO_Config.AltFunc   = 5;
    GPIO_Init(&GPIO_SPI2);

    GPIO_SPI2.pGPIO                 = GPIOC;
    GPIO_SPI2.GPIO_Config.PinNumber = 2;  
    GPIO_Init(&GPIO_SPI2);

    GPIO_SPI2.pGPIO                 = GPIOC;
    GPIO_SPI2.GPIO_Config.PinNumber = 3;  
    GPIO_Init(&GPIO_SPI2);

    GPIOA->MODER |= 1 << 10;
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

void SPI2_Init(){
    RCC->APB1ENR |= (1 << 14);

    SPI_Handle_Type SPI2_Handle;

    SPI2_Handle.pSPI =SPI2;
    SPI2_Handle.SPI_Config.DeviceMode = SPI_DEVICE_MODE_MASTER;
    SPI2_Handle.SPI_Config.DFF = SPI_DFF_8BITS;
    SPI2_Handle.SPI_Config.SSM = 1;
    SPI2_Handle.SPI_Config.BusConfig = SPI_BUS_CONFIG_FD;
    SPI2_Handle.SPI_Config.SclkSpeed = SPI_SCLK_SPEED_DIV128;

    SPI_Init(&SPI2_Handle);
}

int main(void) {
    Clock_Init();       /* Initialize the system clock to use HSE */ 
    GPIOs_Init();       /* Initialize GPIOB for pin 14 output */ 
    SPI2_Init();

    char string[] = "hello world\n";

    while (1) {
        GPIO_WritePin(GPIOA, 5, 1);
        SPI_SendData(SPI2, (uint8_t *)string, sizeof(string) - 1);
        GPIO_WritePin(GPIOA, 5, 0);
        delay_ms(100);
    }
}

