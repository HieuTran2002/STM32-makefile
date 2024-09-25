#include "stm32f411xx.h"
#include "stm32f411xx_clock.h"
#include "stm32f411xx_gpio_driver.h"
#include "stm32f411xx_spi_driver.h"
#include <stdint.h>


void GPIOs_Init(void) {
    GPIOA_PCLK_EN();
    GPIOB_PCLK_EN();
    GPIOC_PCLK_EN();

    GPIO_Handle_Type SPI_PinConfig;

    SPI_PinConfig.pGPIO                                 = GPIOB;
    SPI_PinConfig.GPIO_Config.PinMode                   = GPIO_MODER_OUTPUT;
    SPI_PinConfig.GPIO_Config.AltFunc                   = 5;
    SPI_PinConfig.GPIO_Config.Pull                      = GPIO_PUPDR_NONE;
    SPI_PinConfig.GPIO_Config.OType                     = GPIO_OTYPE_PP;
    SPI_PinConfig.GPIO_Config.Speed                     = GPIO_OSPEED_FAST;

    /* NSS */
    SPI_PinConfig.GPIO_Config.PinNumber                 = SPI2_NSS;
    GPIO_Init(&SPI_PinConfig);

    /* SCLK */
    SPI_PinConfig.GPIO_Config.PinNumber                 = SPI2_SCK;
    GPIO_Init(&SPI_PinConfig);

    /* MISO */
    SPI_PinConfig.GPIO_Config.PinNumber                 = SPI2_MISO;
    GPIO_Init(&SPI_PinConfig);

    /* NSS */
    SPI_PinConfig.GPIO_Config.PinNumber                 = SPI2_MOSI;
    GPIO_Init(&SPI_PinConfig);


    GPIO_Handle_Type GPIO_Output                        = {0};
    GPIO_Output.pGPIO                                   = GPIOA;
    GPIO_Output.GPIO_Config.PinNumber                   = 5;
    GPIO_Output.GPIO_Config.PinMode                     = GPIO_MODER_OUTPUT;
    GPIO_Init(&GPIO_Output);
}


void HSE_Init(void) {
    /* Enable HSE */
    RCC->CR |= (1 << 16);

    /* Check HSE readiness */
    while (!(RCC->CR & (1 << 17)));

    /* Choose HSE as clock system */
    RCC->CFGR |= 0b01;

    FLASH->ACR = (1<<8) | (1<<9)| (1<<10)| (5<<0);
}

int main(void) {
    HSE_Init();
    GPIOs_Init();

    GPIO_WritePin(GPIOA, 5, 1);
    while (1) {
        GPIOB->ODR ^= (0xF << 12);
        delay_ms(100);
    }
    return 0;
}
