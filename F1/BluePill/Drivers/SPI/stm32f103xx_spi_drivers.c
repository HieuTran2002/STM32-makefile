#include "stm32f103xx_spi_drivers.h"
#include <stdint.h>

void SPI_Init(SPI_Handle_Type* pSPI_Handle){
    /* Enable SPI Clock */
    if (pSPI_Handle->pSPI == SPI1) {
        RCC->APB2ENR |= (1 << 12);  
    }
    else if (pSPI_Handle->pSPI == SPI2) {
        RCC->APB1ENR |= (1 << 14);  
    }

    uint16_t temp = 0;

    /* CPHA */
    temp |= pSPI_Handle->SPI_Config.CPHA;

    /* CPOL */
    temp |= pSPI_Handle->SPI_Config.CPOL << 1;

    /* Device mode */
    temp |= pSPI_Handle->SPI_Config.DeviceMode << 2;

    /* Baudrate */
    temp |= pSPI_Handle->SPI_Config.SclkSpeed << 3;

    /* Software slave management */
    if (pSPI_Handle->SPI_Config.SSM) {
        /* Enable SSM, and set SSI */
        temp |= 1 << 8;
        temp |= 1 << 9;
    }
    else{
        pSPI_Handle->pSPI->CR2 |= 1 << 2;
    }

    /* DFF */
    temp |= pSPI_Handle->SPI_Config.DFF << 11;

    /* Bus Config */
    if (pSPI_Handle->SPI_Config.BusConfig == SPI_BUS_CONFIG_HD) {
        temp |= 1 << 15;
    }
    else if (pSPI_Handle->SPI_Config.BusConfig == SPI_BUS_CONFIG_SIMPLES_RXONLY) {
        temp &=  ~(1 << 15);
        temp |= 1 << 10;
    }

    pSPI_Handle->pSPI->CR1 = temp;
}

void SPI_SendData(SPI_TypeDef* pSPI, uint8_t* pTxBuffer, uint8_t Len){
    while (Len) {
        /* 8 bits */
        *(uint8_t *)&pSPI->DR = *pTxBuffer;
        Len--;
        pTxBuffer++;

        /* wait for TX buffer to be emptied */
        while (!(pSPI->SR & (1 << 1)));

        /* Wait until SPI is not busy */
        while (pSPI->SR & (1 << 7));
    }
}

void SPI_RecieveData(SPI_TypeDef* pSPI, uint8_t* pTxBuffer, uint8_t Len){
    while (Len) {
        /* Wait 'til RX buffer become not empty */
        while (!(pSPI->SR && 1));

        if (pSPI->CR1 & (1 << 11)) {
            /* 16 bits */
            *(uint16_t *)pTxBuffer = pSPI->DR;
            Len-=2;
            pTxBuffer+=2;
        }
        else{
            /* 8 bits */
            *pTxBuffer = (uint8_t)pSPI->DR;
            Len--;
            pTxBuffer++;
        }
    }
}
