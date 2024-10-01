#include "stm32f411xx_spi_driver.h"
#include <stdint.h>
#include "stddef.h"

// #define NULL ((void*)0)

void static SPI_TXE_IT_Handle(SPI_Handle_Type* pSPI_Handle);
void static SPI_RXNE_IT_Handle(SPI_Handle_Type* pSPI_Handle);
void static SPI_OVR_IT_Handle(SPI_Handle_Type* pSPI_Handle);



void SPI_Init(SPI_Handle_Type* pSPI_Handle){
    uint32_t temp = 0;

    temp |= pSPI_Handle->SPI_Config.CPHA;
    temp |= pSPI_Handle->SPI_Config.CPOL << SPI_CR1_CPOL;
    temp |= pSPI_Handle->SPI_Config.DeviceMode << SPI_CR1_MSTR;
    temp |= pSPI_Handle->SPI_Config.SclkSpeed << SPI_CR1_BR;    
    temp |= pSPI_Handle->SPI_Config.SSM << SPI_CR1_SSM;
    temp |= pSPI_Handle->SPI_Config.DFF << SPI_CR1_DFF;

    /* SSI */
    if (pSPI_Handle->SPI_Config.SSM) {
        temp |= (1 << SPI_CR1_SSI);
    }
    
    /* BUS */
    if (pSPI_Handle->SPI_Config.BusConfig == SPI_BUS_CONFIG_FD) {
        temp &=  ~(1 << SPI_CR1_BIDIMODE);
    }
    else if (pSPI_Handle->SPI_Config.BusConfig == SPI_BUS_CONFIG_HD) {
        temp |= 1 << SPI_CR1_BIDIMODE;
    }
    else if (pSPI_Handle->SPI_Config.BusConfig == SPI_BUS_CONFIG_SIMPLES_RXONLY) {
        temp &=  ~(1 << SPI_CR1_BIDIMODE);
        temp |= 1 << SPI_CR1_RXONLY;
    }

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
        while (!(pSPI->SR & (1 << SPI_SR_TXE)));

        /* Check DFF */
        if (pSPI->CR1 & (1 << SPI_CR1_DFF)) {
            /* 16 bits */
            pSPI->DR = *((uint16_t*)pTxBuffer);
            Len-=2;
            pTxBuffer+=2;
        }
        else{
            /* 8 bits */
            *(uint8_t *)&pSPI->DR = *pTxBuffer;
            Len--;
            pTxBuffer++;
        }

        while (pSPI->SR & (1 << SPI_SR_BSY));
    }
}

void SPI_RecieveData(SPI_Type* pSPI, uint8_t* pRxBuffer, uint8_t Len){
    while (Len > 0) {
        /* Wait for RXNE to be reset */
        while (!(pSPI->SR && 0b1));

        /* Check DFF */
        if (pSPI->CR1 & (1 << 11)) {
            /* 16 bits */
            *pRxBuffer = (uint16_t)pSPI->DR;
            Len-=2;
            pRxBuffer+=2;
        }
        else{
            /* 8 bits */
            *pRxBuffer = (uint8_t)pSPI->DR;
            Len--;
            pRxBuffer++;
        }
    }

}

uint8_t SPI_SendDataIT(SPI_Handle_Type* pSPI_Handle, uint8_t* pTxBuffer, uint8_t Len){
    uint8_t state = pSPI_Handle->TxState;
    if (state != SPI_BUSY_IN_RX) {
        pSPI_Handle->pTxBuffer = pTxBuffer;
        pSPI_Handle->TxLen = Len;

        pSPI_Handle->TxState = SPI_BUSY_IN_TX;

        /* Set TXEIE */
        pSPI_Handle->pSPI->CR2 |= 1 << SPI_CR2_TXEIE;
    }
    return state;
}

void SPI_IRQHandling(SPI_Handle_Type* pSPI_Handle){
    /* Check TXE */
    uint8_t isTXEIE = (pSPI_Handle->pSPI->SR & (1 << SPI_SR_TXE)) && (pSPI_Handle->pSPI->CR2 & (1 << SPI_CR2_TXEIE));

    if (isTXEIE) {
        SPI_TXE_IT_Handle(pSPI_Handle);
    }

    /* check RXNE */
    uint8_t isRXNE = (pSPI_Handle->pSPI->SR & (1 << SPI_SR_RXNE)) && (pSPI_Handle->pSPI->CR2 & (1 << SPI_CR2_RXNEIE));
    if (isRXNE) {
        SPI_RXNE_IT_Handle(pSPI_Handle); 
    }

    uint8_t isOVR = (pSPI_Handle->pSPI->SR & (1 << SPI_SR_OVR)) && (pSPI_Handle->pSPI->SR & (SPI_CR2_ERRIE));
    if (isOVR) {
        SPI_OVR_IT_Handle(pSPI_Handle);
    }
}

void static SPI_TXE_IT_Handle(SPI_Handle_Type* pSPI_Handle){
    /* Check DFF */
    if (pSPI_Handle->pSPI->CR1 & (1 << SPI_CR1_DFF)) {
        /* 16 bits */
        pSPI_Handle->pSPI->DR = *(uint16_t *)(pSPI_Handle->pTxBuffer);
        pSPI_Handle->TxLen-=2;
        pSPI_Handle->pTxBuffer+=2;
    }
    else{
        /* 8 bits */
        pSPI_Handle->pSPI->DR = *(pSPI_Handle->pTxBuffer);
        pSPI_Handle->TxLen--;
        pSPI_Handle->pTxBuffer++;
    }
    if (! pSPI_Handle->TxLen) {
        pSPI_Handle->pSPI->CR2 &= ~(1 << SPI_CR2_TXEIE);
        pSPI_Handle->pTxBuffer = NULL;
    }
}

void static SPI_RXNE_IT_Handle(SPI_Handle_Type* pSPI_Handle){
    /* Check DFF */
    if (pSPI_Handle->pSPI->CR1 & (1 << SPI_CR1_DFF)) {
        /* 16 bits */
        *((uint16_t*)pSPI_Handle->pRxBuffer) = (uint16_t)pSPI_Handle->pSPI->DR;
        pSPI_Handle->RxLen-=2;
        pSPI_Handle->pRxBuffer+=2;
    }
    else{
        /* 8 bits */
        *(pSPI_Handle->pRxBuffer) = (uint8_t)pSPI_Handle->pSPI->DR;
        pSPI_Handle->RxLen--;
        pSPI_Handle->pRxBuffer++;
    }

    if (! pSPI_Handle->RxLen) {
        pSPI_Handle->pSPI->CR2 &= ~(1 << SPI_CR2_RXNEIE);
        pSPI_Handle->pRxBuffer = NULL;
    }
}

void static SPI_OVR_IT_Handle(SPI_Handle_Type* pSPI_Handle){
    uint8_t temp;
    if (pSPI_Handle->TxState != SPI_BUSY_IN_TX) {
        temp = pSPI_Handle->pSPI->DR;
        temp = pSPI_Handle->pSPI->SR;
    }
    (void)temp;
}

void SPI_CloseTranmission(SPI_Handle_Type* pSPI_Handle){
    pSPI_Handle->pSPI->CR2 &= ~(1 << SPI_CR2_TXEIE);
    pSPI_Handle->pTxBuffer = NULL;
    pSPI_Handle->TxLen = 0;
    pSPI_Handle->TxState = SPI_READY;
}

void SPI_CloseReception(SPI_Handle_Type* pSPI_Handle){
    pSPI_Handle->pSPI->CR2 &= ~(1 << SPI_CR2_RXNEIE);
    pSPI_Handle->pRxBuffer = NULL;
    pSPI_Handle->RxLen = 0;
    pSPI_Handle->RxState = SPI_READY;
}

void __attribute__((weak)) SPI_ApplicationCallback(SPI_Handle_Type* pSPI_Handle, uint8_t AppEv){

}
