#include "stm32f411xx_spi_driver.h"
#include <stdint.h>


void SPI_Init(SPI_Handle_Type* pSPI_Handle){
    uint32_t temp = 0;

    temp |= pSPI_Handle->SPI_Config.CPHA << 0;
    temp |= pSPI_Handle->SPI_Config.CPOL << 1;
    temp |= pSPI_Handle->SPI_Config.DeviceMode << 2;
    temp |= pSPI_Handle->SPI_Config.SclkSpeed << 3;    
    temp |= pSPI_Handle->SPI_Config.SSM << 9;
    temp |= pSPI_Handle->SPI_Config.DFF << 11;

    /* SSI */
    if (pSPI_Handle->SPI_Config.SSM) {
        temp |= (1 << 8);
    }
    
    /* BUS */
    if (pSPI_Handle->SPI_Config.BusConfig == SPI_BUS_CONFIG_FD) {
        temp &=  ~(1 << 15);
    }
    else if (pSPI_Handle->SPI_Config.BusConfig == SPI_BUS_CONFIG_HD) {
        temp |= 1 << 15;
    }
    else if (pSPI_Handle->SPI_Config.BusConfig == SPI_BUS_CONFIG_SIMPLES_RXONLY) {
        temp &=  ~(1 << 15);
        temp |= 1 << 10;
    }

    temp |= (1 << 6);

    pSPI_Handle->pSPI->CR1 = temp;
}

/**
 * Send Data over SPI
 * @brief Send data over SPI
 *
 * @param pSPI      - SPI pointer.
 * @param pTxBuffer - Pointer to data.
 * @param Len       - Lenght of data.
 *
 * @return None
 */
void SPI_SendData(SPI_Type* pSPI, uint8_t* pTxBuffer, uint8_t Len){
    while (Len > 0) {
        /* wait for TX buffer to be emptied */
        while (!(pSPI->SR & (1 << 1)));

        /* Check DFF */
        if (!(pSPI->CR1 & (1 << 9))) {
            /* 16 bits */
            pSPI->DR = *((uint16_t*)pTxBuffer);
            Len--;
            Len--;
            (uint16_t*)pTxBuffer++;
        }
        else{
            /* 8 bits */
            *(uint8_t *)&pSPI->DR = *pTxBuffer;
            Len--;
            pTxBuffer++;
        }

        while (pSPI->SR & (1 << 7));
    }
}
