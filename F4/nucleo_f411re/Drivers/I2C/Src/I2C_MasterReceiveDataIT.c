#include "bsp.h"
#include "stm32f411xx_i2c_driver.h"

uint8_t I2C_MasterReceiveDataIT(I2C_Handle_Type *pI2C_Handle,uint8_t *pRxBuffer, uint32_t Len, uint8_t SlaveAddr,uint8_t Sr)
{

	uint8_t busystate = pI2C_Handle->TxRxState;

    
	if(pI2C_Handle->TxRxState == I2C_FREE)
	{
		pI2C_Handle->pRxBuffer = pRxBuffer;
		pI2C_Handle->RxLen = Len;
		pI2C_Handle->TxRxState = I2C_RX_BUSY;
		pI2C_Handle->RxSize = Len; //Rxsize is used in the ISR code to manage the data reception 
		pI2C_Handle->DevAddr = SlaveAddr;
		pI2C_Handle->Sr = Sr;

		/* Implement code to Generate START Condition */
        pI2C_Handle->pI2C->CR1 |= (1 << I2C_CR1_START);

		//Implement the code to enable ITBUFEN Control Bit
		pI2C_Handle->pI2C->CR2 |= ( 1 << I2C_CR2_ITBUFEN);

		//Implement the code to enable ITEVFEN Control Bit
		pI2C_Handle->pI2C->CR2 |= ( 1 << I2C_CR2_ITEVTEN);

		//Implement the code to enable ITERREN Control Bit
		pI2C_Handle->pI2C->CR2 |= ( 1 << I2C_CR2_ITERREN);

        busystate = I2C_RX_BUSY;
        LED_TOGGLE();
	}

	return busystate;
}
