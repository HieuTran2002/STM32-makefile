#include "stm32f4xx_uart_driver.h"

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

	}else if (pUSARTHandle->USART_Config.USART_ParityControl == USART_PS_EVEN)
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
}
