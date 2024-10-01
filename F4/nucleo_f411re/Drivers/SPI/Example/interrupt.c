#include "stm32f411xx.h"
#include "stm32f411xx_spi_driver.h"
#include "stm32f411xx_gpio_driver.h"
#include "stm32f411xx_clock.h"
#include "stdint.h"

#define BTN_PRESSED            !((GPIOC->IDR >> 13) & 1)

void Clock_Config(void){
    RCC->CR |= 1;
    while (!((RCC->CR >> 1) & 1));
}

void GPIO_Config(){
    /* Enable GPIOA->D */
    RCC->AHB1ENR |= 0b111;
    GPIOA->MODER |= 1 << 10;
}

void SPI_Pin_Config(){
    GPIO_Handle_Type SPI_Pin;

    SPI_Pin.pGPIO = GPIOB;

    SPI_Pin.GPIO_Config.PinNumber       = 9;
    SPI_Pin.GPIO_Config.PinMode         = GPIO_MODER_ALTFNC;
    SPI_Pin.GPIO_Config.AltFunc         = 5;
    SPI_Pin.GPIO_Config.Speed           = GPIO_OSPEED_FAST;
    SPI_Pin.GPIO_Config.Pull            = GPIO_PUPDR_NONE;
    GPIO_Init(&SPI_Pin);

    SPI_Pin.GPIO_Config.PinNumber       = 10;
    GPIO_Init(&SPI_Pin);

    SPI_Pin.pGPIO                       = GPIOC;
    SPI_Pin.GPIO_Config.PinNumber       = 2;
    GPIO_Init(&SPI_Pin);

    SPI_Pin.GPIO_Config.PinNumber       = 3;
    GPIO_Init(&SPI_Pin);
}

void SPI_Config(void){
    SPI2_PCLK_EN();
    SPI_Handle_Type SPI_Config;

    SPI_Config.pSPI                     = SPI2;
    SPI_Config.SPI_Config.DeviceMode    = SPI_DEVICE_MODE_MASTER;
    SPI_Config.SPI_Config.DFF           = 0;
    SPI_Config.SPI_Config.BusConfig     = SPI_BUS_CONFIG_FD;
    SPI_Config.SPI_Config.SclkSpeed     = 7;
    SPI_Config.SPI_Config.CPOL          = 0;
    SPI_Init(&SPI_Config);

    SPI2->CR2 |= 1 << 2;
}
int main(void){
    Clock_Config();
    GPIO_Config();
    SPI_Pin_Config();
    SPI_Config();
    char string[] = "hello world";
    while (1) {
        if (BTN_PRESSED){
            SPI2->CR1 |= 1 << SPI_CR1_SPE;

            delay_ms(1);

            SPI_SendData(SPI2, (uint8_t *)string, sizeof(string) - 1); 

            delay_ms(1);

            SPI2->CR1 &= ~(1 << SPI_CR1_SPE);
            while (BTN_PRESSED);
        }
    }
    return 0;
}
