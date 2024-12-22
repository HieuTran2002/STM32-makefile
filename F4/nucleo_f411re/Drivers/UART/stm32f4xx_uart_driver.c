#include "stm32f4xx_uart_driver.h"
#include "stm32f411xx_clock.h"
#include "stm32f411xx.h"
#include <stdint.h>

uint8_t USART_GetFlag(USART_TypeDef* pUSART, uint8_t StatusFlagName){
    return (pUSART->SR >> StatusFlagName) & 0x1;
}

void USART_PeripheralControl(USART_TypeDef* pUSART, uint8_t EnOrDi){
    if (EnOrDi) {
        pUSART == USART2 ? RCC->APB1ENR |= (1 << 17) : 0;
        pUSART == USART1 ? RCC->APB2ENR |= (1 << 4)  : 0;
        pUSART == USART6 ? RCC->APB2ENR |= (1 << 5)  : 0;
    }
}

void USART_Init(USART_Handle_Type *pUSARTHandle)
{
    uint32_t tempreg=0;

    /* Implement the code to enable the Clock for given USART peripheral */ 
    /* Enable USART Tx and Rx engines according to the USART_Mode configuration item */
    if ( pUSARTHandle->USART_Config.USART_Mode == USART_MODE_ONLY_RX)
    {
        /* Implement the code to enable the Receiver bit field */
        tempreg|= (1 << USART_CR1_RE);
    }else if (pUSARTHandle->USART_Config.USART_Mode == USART_MODE_ONLY_TX)
    {
        /* Implement the code to enable the Transmitter bit field  */
        tempreg |= ( 1 << USART_CR1_TE );

    }else if (pUSARTHandle->USART_Config.USART_Mode == USART_MODE_TXRX)
    {
        /* Implement the code to enable the both Transmitter and Receiver bit fields */
        tempreg |= ( ( 1 << USART_CR1_TE) | ( 1 << USART_CR1_RE) );
    }

    /* Implement the code to configure the Word length configuration item */
    tempreg |= pUSARTHandle->USART_Config.USART_WordLength <<  USART_CR1_M;

    //Configuration of parity control bit fields
    if ( pUSARTHandle->USART_Config.USART_ParityControl == USART_PS_EVEN)
    {
        /* Implement the code to enale the parity control  */
        tempreg |= ( 1 << USART_CR1_PCE);

    }else if (pUSARTHandle->USART_Config.USART_ParityControl == USART_PS_ODD)
    {
        /* Implement the code to enable the parity control  */
        tempreg |= ( 1 << USART_CR1_PCE);

        /* Implement the code to enable ODD parity  */
        tempreg |= ( 1 << USART_CR1_PS);
    }

    /* Program the CR1 register  */
    pUSARTHandle->pUSART->CR1 = tempreg;

    tempreg=0;

    /* Implement the code to configure the number of stop bits inserted during USART frame transmission  */
    tempreg |= pUSARTHandle->USART_Config.USART_NoOfStopBits << USART_CR2_STOP;
    pUSARTHandle->pUSART->CR2 = tempreg;

    tempreg=0;

    /* Configuration of USART hardware flow control  */
    if ( pUSARTHandle->USART_Config.USART_HWFlowControl == USART_HW_FLOW_CTRL_CTS)
    {
        /* Implement the code to enable CTS flow control  */
        tempreg |= ( 1 << USART_CR3_CTSE);


    }else if (pUSARTHandle->USART_Config.USART_HWFlowControl == USART_HW_FLOW_CTRL_RTS)
    {
        /* Implement the code to enable RTS flow control  */
        tempreg |= ( 1 << USART_CR3_RTSE);

    }else if (pUSARTHandle->USART_Config.USART_HWFlowControl == USART_HW_FLOW_CTRL_CTS_RTS)
    {
        /* Implement the code to enable both CTS and RTS Flow control  */
        tempreg |= ( 1 << USART_CR3_RTSE) | ( 1 << USART_CR3_CTSE);
    }

    pUSARTHandle->pUSART->CR3 = tempreg;

    /* Implement the code to configure the baud rate */
    USART_SetBaudRate(pUSARTHandle->pUSART, pUSARTHandle->USART_Config.USART_Baud);

    /* Enable USART peripheral */
    pUSARTHandle->pUSART->CR1 |= (1 << USART_CR1_UE);
}

void USART_SetBaudRate(USART_TypeDef *pUSART, uint32_t BaudRate)
{
    //Variable to hold the APB clock
    uint32_t PCLKx;

    uint32_t usartdiv;

    //variables to hold Mantissa and Fraction values
    uint32_t M_part,F_part;

    uint32_t tempreg=0;

    //Get the value of APB bus clock in to the variable PCLKx
    if(pUSART == USART1 || pUSART == USART6)
    {
        PCLKx = GetAPB2_Clock();
    }else
    {
        PCLKx = GetAPB1_Clock();
    }

    //Check for OVER8 configuration bit
    if(pUSART->CR1 & (1 << USART_CR1_OVER8))
    {
        //OVER8 = 1 , over sampling by 8
        usartdiv = ((25 * PCLKx) / (2 *BaudRate));
    }else
    {
        //over sampling by 16
        usartdiv = ((25 * PCLKx) / (4 *BaudRate));
    }

    //Calculate the Mantissa part
    M_part = usartdiv/100;

    //Place the Mantissa part in appropriate bit position . refer USART_BRR
    tempreg |= M_part << 4;

    //Extract the fraction part
    F_part = (usartdiv - (M_part * 100));

    //Calculate the final fractional
    if(pUSART->CR1 & ( 1 << USART_CR1_OVER8))
    {
        //OVER8 = 1 , over sampling by 8
        F_part = ((( F_part * 8)+ 50) / 100)& ((uint8_t)0x07);

    }else
    {
        //over sampling by 16
        F_part = ((( F_part * 16)+ 50) / 100) & ((uint8_t)0x0F);

    }

    //Place the fractional part in appropriate bit position . refer USART_BRR
    tempreg |= F_part;

    //copy the value of tempreg in to BRR register
    pUSART->BRR = tempreg;
}


void USART_SendData(USART_Handle_Type *pUSARTHandle, uint8_t *pTxBuffer, uint32_t Len)
{
    uint16_t *pdata;

    /* Loop over until "Len" number of bytes are transferred */
    for(uint32_t i = 0 ; i < Len; i++)
    {
        /* Implement the code to wait until TXE flag is set in the SR */
        while(! USART_GetFlag(pUSARTHandle->pUSART, USART_SR_TXE)) {};

        /* Check the USART_WordLength item for 9BIT or 8BIT in a frame */
        if(pUSARTHandle->USART_Config.USART_WordLength == USART_WORDLEN_9BITS)
        {
            /* if 9BIT, load the DR with 2bytes masking the bits other than first 9 bits  */
            pdata = (uint16_t *) pTxBuffer;

            pUSARTHandle->pUSART->DR = (*pdata & (uint16_t)0x01FF);

            /* check for USART_ParityControl */
            if(pUSARTHandle->USART_Config.USART_ParityControl == 0)
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
            if (pUSARTHandle->USART_Config.USART_ParityControl == USART_PS_NONE) {
                pUSARTHandle->pUSART->DR = (*pTxBuffer & 0xFF);
            }
            else {
                pUSARTHandle->pUSART->DR = (*pTxBuffer & 0x7F);
            }
            /* Implement the code to increment the buffer address */
            pTxBuffer += 1;
        }
    }
    /* Implement the code to wait till TC flag is set in the SR */
    while( ! USART_GetFlag(pUSARTHandle->pUSART, USART_SR_TC));
    pUSARTHandle->pUSART->SR &= ~(1 << USART_SR_TC);
}

void USART_ReceiveData(USART_Handle_Type *pUSARTHandle, uint8_t *pRxBuffer, uint32_t Len)
{
    /* Loop over until "Len" number of bytes are transferred */
    for(uint32_t i = 0 ; i < Len; i++)
    {
        /* Implement the code to wait until RXNE flag is set in the SR */
        while (!USART_GetFlag(pUSARTHandle->pUSART, USART_SR_RXNE));

        if(pUSARTHandle->USART_Config.USART_WordLength == USART_WORDLEN_9BITS)
        {
            /* We are going to receive 9bit data in a frame */

            /* check are we using USART_ParityControl control or not */
            if(pUSARTHandle->USART_Config.USART_ParityControl == 0)
            {
                /* No parity is used. so, all 9bits will be of user data */

                /* read only first 9 bits. so, mask the DR with 0x01FF */
                *((uint16_t*) pRxBuffer) = (pUSARTHandle->pUSART->DR  & (uint16_t)0x01FF);

                /* Now increment the pRxBuffer two times */
                pRxBuffer+=2;

            }
            else
            {
                /* Parity is used, so, 8bits will be of user data and 1 bit is parity */
                *pRxBuffer = (pUSARTHandle->pUSART->DR  & (uint8_t)0xFF);

                /* Increment the pRxBuffer */
                pRxBuffer++;
            }
        }
        else
        {
            // We are going to receive 8bit data in a frame

            //check are we using USART_ParityControl control or not
            if(pUSARTHandle->USART_Config.USART_ParityControl == 0)
            {
                //No parity is used , so all 8bits will be of user data

                //read 8 bits from DR
                *pRxBuffer = (pUSARTHandle->pUSART->DR);
            }

            else
            {
                //Parity is used, so , 7 bits will be of user data and 1 bit is parity

                //read only 7 bits , hence mask the DR with 0X7F
                *pRxBuffer = (uint8_t)(pUSARTHandle->pUSART->DR);

            }
            //increment the pRxBuffer
            pRxBuffer++;
        }
    }

}
