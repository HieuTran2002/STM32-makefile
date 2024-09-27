#include "stm32f411xx.h"
#include "stm32f411xx_clock.h"
#include <stdint.h>

void SPI_Transmit(SPI_Type *pSPIx, uint8_t *pTxBuffer, uint32_t length) {
    while (length > 0) {
        // Wait until TXE is set (Transmit buffer empty)
        while (!(pSPIx->SR & (1 << 1)));

        // Send data

        if (pSPIx->CR1 & (1 << 11)){
            pSPIx->DR = *(uint16_t *)pTxBuffer;
            pTxBuffer++;
            length--;
        }
        else{
            pSPIx->DR = *pTxBuffer;
            pTxBuffer++;
            length--;

        }
    }

    // Wait until BSY flag is cleared (Transmission completed)
    while (pSPIx->SR & (1 << 7));
}

/* Enable GPIOB and SPI2 Peripheral Clocks */
void SPI2_GPIO_Init(void){
    /* Enable GPIO Clock */

    /* 
     * PC2  : SPI2_MISO
     * PC3  : SPI2_MOSI
     * PB10 : SPI2_SCK
     */

    GPIOC->MODER    &= ~(1 << 4 | 1 << 6);
    GPIOC->MODER    |= 2 << 4 | 2 << 6;

    GPIOC->AFR[0]   &= ~(0xF << 8 | 0xF << 12);    
    GPIOC->AFR[0]   |= 5 << 8;
    GPIOC->AFR[0]   |= 5 << 12;

    GPIOC->PUPDR |= 0b11 << 4 | 0b11 << 6;

    GPIOB->MODER    &= ~(1 << 20);
    GPIOB->MODER    |= 2 << 20;
    GPIOB->AFR[1]   |= 5 << 8;  

    GPIOB->PUPDR |= 0b11 << 20;


}

/* Initialize SPI2 Peripheral */
void SPI2_Init(void){
    uint32_t temp = 0;

    /* Enable SPI2 Clock */
    RCC->APB1ENR |= (1 << 14);  // Enable SPI2 clock

    /* Configure SPI2 as Master, 8-bit data, SSM enabled, CPOL/CPHA low */
    temp = 0;  // Clear control register

    temp |= (1 << 2);  /* Device mode | master or BET */ 

    temp |= (7 << 3); /* Baudrate */

    temp &= ~(1 << 11); /* DFF 0 = 8 bits, 1 = 16 bits */

    temp |= (1 << 9);  /* SSM = 1 (Software Slave Management) */ 

    temp |= (1 << 8);  /* SSI = 1 (Internal Slave Select) */ 

    temp |= (1 << 6);  // SPE = 1 (Enable SPI)

    SPI2->CR1 |= temp;
}


/* Main function */
int main(void){
    RCC->CR |= 1;

    RCC->AHB1ENR |= 0b1111;

    GPIOA->MODER |= 1 << 10;

    SPI2_GPIO_Init();
    SPI2_Init();

    /* Send data over SPI2 */
    char string[] = "hello world\n";

    while (1){
        GPIOA->ODR |= (1 << 5);
        SPI_Transmit(SPI2, (uint8_t *)string, 12);
        GPIOA->ODR &= ~(1 << 5);
        delay_ms(100);
    }
}
