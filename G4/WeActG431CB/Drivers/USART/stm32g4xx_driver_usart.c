#include "stm32g4xx_driver_usart.h"
#include "bsp.h"
#include "stm32g431xx.h"
#include <stdint.h>

void USART_PCLK_Control(USART_TypeDef* pUSART, uint8_t EnOrDi){
    if (EnOrDi) {
        if (pUSART == USART2) { RCC->APB1ENR1 |= (1 << 17); }
    }
}

void USART_Init(USART_Handle_Type* pUSART_Handle){
    USART_PCLK_Control(pUSART_Handle->pUSART, 1);

    uint32_t temp = 0;

    if (pUSART_Handle->USART_Config.Mode == USART_TX_ONLY) {
        temp |= (1 << USART_CR1_TE_Pos);
    }
    else if (pUSART_Handle->USART_Config.Mode == USART_RX_ONLY) {
        temp |= (1 << USART_CR1_RE_Pos);
    }
    else if(pUSART_Handle->USART_Config.Mode == USART_RXTX){
        temp |= (1 << USART_CR1_RE_Pos);
        temp |= (1 << USART_CR1_TE_Pos);
    }

    temp |= (pUSART_Handle->USART_Config.Wordlen << USART_CR1_M_Pos);

    if (pUSART_Handle->USART_Config.Parity != USART_Parity_None) {
        temp |= (1 << USART_CR1_PS_Pos);
        temp |= (pUSART_Handle->USART_Config.Parity << USART_CR1_PCE_Pos);
    }

    pUSART_Handle->pUSART->CR1 = temp;

    /* CR2 */
    temp = 0;
    temp |= (pUSART_Handle->USART_Config.StopBits << USART_CR2_STOP_Pos);
    pUSART_Handle->pUSART->CR2 = temp;


    /* BRR */
    temp = 0;
    temp = pUSART_Handle->USART_Config.Baudrate;
    pUSART_Handle->pUSART->BRR |= 3;
    pUSART_Handle->pUSART->BRR |= (0x68 << 4);


    /* Enable USART */
    pUSART_Handle->pUSART->CR1 |= 1;
}

uint8_t USART_GetFlag(USART_TypeDef* pUSART, uint8_t StatusFlagName){
    return (pUSART->ISR >> StatusFlagName) & 0x1;
}

void USART_SendData(USART_Handle_Type* pUSART_Handle, uint8_t* pTxBuffer, uint8_t Len){
    uint16_t *pdata;

    /* Loop over until "Len" number of bytes are transferred */
    for(uint32_t i = 0 ; i < Len; i++)
    {
        /* Implement the code to wait until TXE flag is set in the SR */
        while(! USART_GetFlag(pUSART_Handle->pUSART, USART_ISR_TXE_Pos)) {};

        /* Check the USART_WordLength item for 9BIT or 8BIT in a frame */
        if(pUSART_Handle->USART_Config.Wordlen == USART_WORDLEN_9)
        {
            /* if 9BIT, load the DR with 2bytes masking the bits other than first 9 bits  */
            pdata = (uint16_t *) pTxBuffer;

            pUSART_Handle->pUSART->TDR = (*pdata & (uint16_t)0x01FF);

            /* check for USART_ParityControl */
            if(pUSART_Handle->USART_Config.Parity == USART_Parity_None)
            {
                /* No parity is used. 9bits of user data will be sent */
                /* Implement the code to increment pTxBuffer twice  */
                pTxBuffer++;
                pTxBuffer++;
            }
            else
            {
                /* The 9th bit will be replaced by parity bit by the hardware */
                pTxBuffer++;
            }
        }
        else
        {
            /* This is 8bit data transfer */
            if (pUSART_Handle->USART_Config.Parity == USART_Parity_None) {
                LED_ON();
                pUSART_Handle->pUSART->TDR = (*pTxBuffer & 0xFF);
            }
            else {
                pUSART_Handle->pUSART->TDR = (*pTxBuffer & 0x7F);
            }
            /* Implement the code to increment the buffer address */
            pTxBuffer += 1;
        }
    }
    /* Implement the code to wait till TC flag is set in the SR */
    while( ! USART_GetFlag(pUSART_Handle->pUSART, USART_ISR_TC_Pos));
    pUSART_Handle->pUSART->ISR &= ~(1 << USART_ISR_TC_Pos);
}
