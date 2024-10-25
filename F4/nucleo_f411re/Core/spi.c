#include "stm32f411xx.h"
#include "stm32f411xx_clock.h"
#include "stm32f411xx_gpio_driver.h"
#include "stm32f411xx_spi_driver.h"
#include <stdint.h>


void SPI2_GPIO_Init(void){
    /* Enable GPIOB Clock (where SPI2 pins are located) */
    RCC->AHB1ENR |= (1 << 1);  // Enable GPIOB clock (for SPI2 pins)

    /* Configure PB13 (SCK), PB14 (MISO), PB15 (MOSI) as Alternate Function */
    GPIOB->MODER &= ~((0b11 << 26) | (0b11 << 28) | (0b11 << 30));  // Clear mode bits for PB13, PB14, PB15
    GPIOB->MODER |= ((0b10 << 26) | (0b10 << 28) | (0b10 << 30));   // Set PB13, PB14, PB15 to AF mode

    /* Set Alternate Function 5 (SPI2) for PB13, PB14, PB15 */
    GPIOB->AFR[1] &= ~((0b1111 << 20) | (0b1111 << 24) | (0b1111 << 28));  // Clear AFR for pins
    GPIOB->AFR[1] |= ((0b0101 << 20) | (0b0101 << 24) | (0b0101 << 28));   // Set AFR for SPI2 (AF5)

    /* Configure the pins as push-pull, high speed */
    GPIOB->OTYPER &= ~((1 << 13) | (1 << 14) | (1 << 15));  // Set pins as push-pull
    GPIOB->OSPEEDR |= ((0b11 << 26) | (0b11 << 28) | (0b11 << 30));  // High speed

    /* Optional: Set pins to no pull-up/pull-down */
    GPIOB->PUPDR &= ~((0b11 << 26) | (0b11 << 28) | (0b11 << 30));  // No pull-up/pull-down
}

void GPIOs_Init(void) {
    GPIOA_PCLK_EN();
    GPIOB_PCLK_EN();

    GPIO_Handle_Type SPI_PinConfig;

    SPI_PinConfig.pGPIO                                 = SPI2_PORT;
    SPI_PinConfig.GPIO_Config.PinMode                   = GPIO_MODER_ALTFNC;
    SPI_PinConfig.GPIO_Config.AltFunc                   = 0b0101;
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

void SPI2_Init(void){
    /* Enable SPI2 Clock */
    RCC->APB1ENR |= (1 << 14);  // Enable SPI2 clock

    /* Configure SPI2 as Master, 8-bit data, SSM enabled, CPOL/CPHA low */
    SPI2->CR1 = 0;  // Clear control register

    SPI2->CR1 |= (1 << 2);  // Master mode
    SPI2->CR1 &= ~(1 << 11);  // 8-bit data frame format
    SPI2->CR1 &= ~(1 << 1);  // CPOL = 0 (Clock polarity low)
    SPI2->CR1 &= ~(1 << 0);  // CPHA = 0 (Clock phase 1st edge)

    /* Baud rate: PCLK/16 (could be adjusted based on your needs) */
    SPI2->CR1 |= (3 << 3);

    /* Enable software slave management (SSM) and set internal slave select (SSI) */
    SPI2->CR1 |= (1 << 9);  // SSM = 1 (Software Slave Management)
    SPI2->CR1 |= (1 << 8);  // SSI = 1 (Internal Slave Select)

    /* Enable SPI2 Peripheral */
    SPI2->CR1 |= (1 << 6);  // SPE = 1 (Enable SPI)
}


void HSE_Init(void) {
    /* Enable HSI */
    RCC->CR |= (1 << 16);

    /* Check HSI readiness */
    while (!(RCC->CR & (1 << 17)));

    /* Choose HSI as clock system */
    RCC->CFGR &= ~1;

    FLASH->ACR = (1<<8) | (1<<9)| (1<<10)| (5<<0);
}

void SPI_Send(SPI_Type *pSPIx, uint8_t *pTxBuffer, uint32_t length) {
    while (length > 0) {
        // Wait until TXE is set (Transmit buffer empty)
        while (!(pSPIx->SR & (1 << 1)));

        // Send data
        pSPIx->DR = *pTxBuffer;
        pTxBuffer++;
        length--;
    }

    // Wait until BSY flag is cleared (Transmission completed)
    while (pSPIx->SR & (1 << 7));
}

int main(void) {
    HSE_Init();

    GPIOA_PCLK_EN();
    GPIOA->MODER |= 1 << 10;
    GPIOA->ODR |= 1 << 5;

    // GPIOs_Init();
    SPI2_GPIO_Init();
    SPI2_Init();


    char string[] = "hello";
    SPI_Send(SPI2, (uint8_t *)string, 5);

    // SPI2_Transmit(7);

    while (1) delay_ms(10);
    return 0;
}
