#include "stm32f411xx_i2c_driver.h"
#include "stm32f411xx.h"
#include "stm32f411xx_clock.h"
#include "stm32f411xx_i2c_driver.h"
#include "stm32f411xx_types.h"
#include <stdint.h>

uint8_t rx_count = 5;

static uint8_t I2C_SR_GetFlag(I2C_Type* pI2C, uint32_t Flag);
static void I2C_ClearADDRFlag(I2C_Handle_Type* pI2CHandle);
static void I2C_ManageACK(I2C_Type* pI2C, uint8_t EnOrDi);
static void I2C_GenerateStartCondition(I2C_Type* pI2C);
static void I2C_GenerateStopCondition(I2C_Type* pI2C);



void I2C_Clock_Control(I2C_Type* pI2C, uint8_t EnOrDi){
    if (EnOrDi) {
        (pI2C == I2C1) ? (RCC->APB1ENR |= (1 << 21)) : 0;
        (pI2C == I2C2) ? (RCC->APB1ENR |= (1 << 22)) : 0;
        (pI2C == I2C3) ? (RCC->APB1ENR |= (1 << 23)) : 0;
    }
    else {
        (pI2C == I2C1) ? RCC->APB1ENR &= ~(1 << 21) : 0;
        (pI2C == I2C2) ? RCC->APB1ENR &= ~(1 << 22) : 0;
        (pI2C == I2C3) ? RCC->APB1ENR &= ~(1 << 23) : 0;
    }
    pI2C->CR1 |= 1;
}

void I2C_Init(I2C_Handle_Type* pI2C_Handle){
    uint16_t tmp = 0;

    // Ensure the WWDG is disabled if you don't intend to use it.
    RCC->APB1ENR &= ~(1 << 11);  // Disable the WWDG clock

    /* Enable peripharel clock */
    I2C_Clock_Control(pI2C_Handle->pI2C, 1);

    /* ACK */
    tmp = (pI2C_Handle->I2C_Config.I2C_ACK_Control << I2C_CR1_ACK);
    pI2C_Handle->pI2C->CR1 &= ~(1 << I2C_CR1_ACK);
    pI2C_Handle->pI2C->CR1 |= tmp;

    I2C_ManageACK(pI2C_Handle->pI2C, 1);

    /* FREQ */
    tmp = 16 << I2C_CR2_FREQ;
    pI2C_Handle->pI2C->CCR &= ~(0x3F << I2C_CR2_FREQ);
    pI2C_Handle->pI2C->CCR |= tmp;

    /* Device address */
    tmp = (pI2C_Handle->I2C_Config.I2C_Device_Address) << 1;
    pI2C_Handle->pI2C->OAR1 &= ~(0x7F << 1);
    pI2C_Handle->pI2C->OAR1 |= tmp;

    /* CCR */
    uint16_t crr_value = 0;
    tmp = 0;

    if (pI2C_Handle->I2C_Config.I2C_SCLSpeed == I2C_SCL_SPEED_STANDARD) {
        crr_value = GetSysTick() / (2 * pI2C_Handle->I2C_Config.I2C_SCLSpeed);
        tmp |= (crr_value & 0xFFF);
    }
    else {
        tmp |= (1 << 15);
        tmp |= (pI2C_Handle->I2C_Config.I2C_FMDutyCycle << 14);
        if      (pI2C_Handle->I2C_Config.I2C_FMDutyCycle == I2C_FM_DUTY_2) {
            crr_value = GetSysTick() / (3 * pI2C_Handle->I2C_Config.I2C_SCLSpeed);
        }
        else if (pI2C_Handle->I2C_Config.I2C_FMDutyCycle == I2C_FM_DUTY_16_9) {
            crr_value = GetSysTick() / (25 * pI2C_Handle->I2C_Config.I2C_SCLSpeed);
        }
        tmp |= (crr_value & 0xFFF);
    }

    pI2C_Handle->pI2C->CCR |= tmp;
}


static uint8_t I2C_SR_GetFlag(I2C_Type* pI2C, uint32_t Flag){
    return ((pI2C->SR1 >> Flag) & 1) ? 1 : 0;
}

/**
 * Move slave address to left by 1 
 *
 * Bit 0 = 1 (read) | 0 (write) 
 * Description.
 */
static void I2C_ExecuteAddressPhase(I2C_Type* pI2C, uint8_t SlaveAddress, uint8_t isRead){
    pI2C->DR = (SlaveAddress << 1 | (isRead ? 1 : 0));
}

static void I2C_GenerateStartCondition(I2C_Type* pI2C){
    /* Generate start */
    pI2C->CR1 |= (1 << I2C_CR1_START);
}

static void I2C_GenerateStopCondition(I2C_Type* pI2C){
    /* Generate start */
    pI2C->CR1 |= (1 << I2C_CR1_STOP);
}

static void I2C_ClearADDRFlag(I2C_Handle_Type* pI2CHandle){
    if (pI2CHandle->pI2C->SR2 && 0b1) {
        /* Device is in slave mode */
        if (pI2CHandle->TxRxState == I2C_RX_BUSY) {
            if (pI2CHandle->RxLen == 1) {
                /* disable ACK */
                I2C_ManageACK(pI2CHandle->pI2C, 0);

                /* clear addr (read sr1 sr2)*/
                (void)pI2CHandle->pI2C->SR1;
                (void)pI2CHandle->pI2C->SR2;
            }
        }
        else{
            (void)pI2CHandle->pI2C->SR1;
            (void)pI2CHandle->pI2C->SR2;
        }
    }
    else {
        /* Device is in master mode */
        (void)pI2CHandle->pI2C->SR1;
        (void)pI2CHandle->pI2C->SR2;
    }
}

static void I2C_ManageACK(I2C_Type* pI2C, uint8_t EnOrDi){
    (EnOrDi) ? (pI2C->CR1 |=  (1 << I2C_CR1_ACK)):
               (pI2C->CR1 &= ~(1 << I2C_CR1_ACK));
}

void I2C_MasterSendata(I2C_Handle_Type* pI2C_Handle, uint8_t* pTxBuffer, uint32_t Len, uint8_t SlaveAddress){
    /* Generate start condition */
    I2C_GenerateStartCondition(pI2C_Handle->pI2C);

    /* Confirm start condition */
    while (!I2C_SR_GetFlag(pI2C_Handle->pI2C, I2C_SR1_SB));

    /* Send address and w/r bit */
    I2C_ExecuteAddressPhase(pI2C_Handle->pI2C, SlaveAddress, 0);

    /* Confirm  */
    while (!I2C_SR_GetFlag(pI2C_Handle->pI2C, I2C_SR1_ADDR));

    /* Clear ADDR flag */
    I2C_ClearADDRFlag(pI2C_Handle);

    /* Send bytes 'til len = 0 */
    while (Len > 0) {
        while (!I2C_SR_GetFlag(pI2C_Handle->pI2C, I2C_SR1_TXE));
        pI2C_Handle->pI2C->DR = *pTxBuffer;
        pTxBuffer++;
        Len--;
    }

    /* Check TXE, BTF flags before generate STOP condition */
    while (!I2C_SR_GetFlag(pI2C_Handle->pI2C, I2C_SR1_TXE));
    while (!I2C_SR_GetFlag(pI2C_Handle->pI2C, I2C_SR1_BTF));

    /* Generate STOP condition */
    I2C_GenerateStopCondition(pI2C_Handle->pI2C);
}



void I2C_MasterReadData(I2C_Handle_Type* pI2C_Handle, uint8_t* pRxBuffer, uint32_t Len, uint8_t SlaveAddress){
    /* Generate start condition */
    I2C_GenerateStartCondition(pI2C_Handle->pI2C);

    /* Confirm start condition */
    while (!I2C_SR_GetFlag(pI2C_Handle->pI2C, I2C_SR1_SB));

    /* Send address and w/r bit */
    I2C_ExecuteAddressPhase(pI2C_Handle->pI2C, SlaveAddress, 1);

    /* Confirm */
    while (!I2C_SR_GetFlag(pI2C_Handle->pI2C, I2C_SR1_ADDR));

    if (Len == 1) {
        /* Generate NACK to stop transmission */
        I2C_ManageACK(pI2C_Handle->pI2C, 0);

        /* Clear ADDR flag */
        I2C_ClearADDRFlag(pI2C_Handle);

        /* wait for RXNE */
        while (!I2C_SR_GetFlag(pI2C_Handle->pI2C, I2C_SR1_RXNE));

        /* Generate stop condition */
        I2C_GenerateStopCondition(pI2C_Handle->pI2C);

        /* Parse data to Rx pointer */
        *pRxBuffer = pI2C_Handle->pI2C->DR;

        return;
    }

    if (Len > 1) {
        /* Clear ADDR flag */
        I2C_ClearADDRFlag(pI2C_Handle);

        /* Enable ACK */
        I2C_ManageACK(pI2C_Handle->pI2C, 1);

        /* Loop until len = 0 */
        for (uint8_t i = Len; i > 0; i--) {
            /* wait for RXNE */
            while (!I2C_SR_GetFlag(pI2C_Handle->pI2C, I2C_SR1_RXNE));


            /* handle the last 2 bytes*/
            if (i == 2) {
                /* Set ACK to low */
                I2C_ManageACK(pI2C_Handle->pI2C, 0);

                /* Generate stop condition */
                I2C_GenerateStopCondition(pI2C_Handle->pI2C);
            }
            /* Save data to rx buffer */
            *pRxBuffer = pI2C_Handle->pI2C->DR;

            /* increase pointer address */
            pRxBuffer++;
        }
    }

    /* Re-enable ACK */
    if (pI2C_Handle->I2C_Config.I2C_ACK_Control == I2C_ACK_DISABLE) {
        I2C_ManageACK(pI2C_Handle->pI2C, 1);
    }
}



static void I2C_CloseTransmittion(I2C_Handle_Type* pI2CHandle){
    /* Disable ITEVTEN and ITBUFEN */
    pI2CHandle->pI2C->CR2 &= ~(1 << I2C_CR2_ITEVTEN);
    pI2CHandle->pI2C->CR2 &= ~(1 << I2C_CR2_ITBUFEN);
    
    /* reset I2C handler */
    pI2CHandle->TxRxState = I2C_FREE;
    pI2CHandle->pTxBuffer = (void*)0;
    pI2CHandle->TxLen = 0;
    I2C_ManageACK(pI2CHandle->pI2C, 1);
}

static void I2C_CloseReception(I2C_Handle_Type* pI2CHandle){
    /* Disable ITEVTEN and ITBUFEN */
    pI2CHandle->pI2C->CR2 &= ~(1 << I2C_CR2_ITEVTEN);
    pI2CHandle->pI2C->CR2 &= ~(1 << I2C_CR2_ITBUFEN);
    
    /* reset I2C handler */
    pI2CHandle->TxRxState = I2C_FREE;
    pI2CHandle->pRxBuffer = (void*)0;
    pI2CHandle->RxLen = 0;
    pI2CHandle->RxSize = 0;
    if (pI2CHandle->I2C_Config.I2C_ACK_Control == I2C_ACK_ENABLE) {
        I2C_ManageACK(pI2CHandle->pI2C, 1);
    }
    I2C_ManageACK(pI2CHandle->pI2C, 1);
}


/*
 * Handle all I2C interrupt event and error
 *
 * @param I2C_Handle_Type
 * @return void
 *
 */
void I2C_EV_IRQHandler(I2C_Handle_Type* pI2C_Handle){
    uint8_t isITEVTEN, isITBUFEN;

    isITEVTEN   = ((pI2C_Handle->pI2C->CR2 >> I2C_CR2_ITEVTEN) & 1);
    isITBUFEN   = ((pI2C_Handle->pI2C->CR2 >> I2C_CR2_ITBUFEN) & 1);

    if (isITEVTEN && ((pI2C_Handle->pI2C->SR1 >> I2C_SR1_SB) && 1)) {
        /* Start condition was sent, SR1 was read */
        if (pI2C_Handle->TxRxState == I2C_TX_BUSY) {
            I2C_ExecuteAddressPhase(pI2C_Handle->pI2C, pI2C_Handle->DevAddr, 0);
        }
        else if (pI2C_Handle->TxRxState == I2C_RX_BUSY) {
            I2C_ExecuteAddressPhase(pI2C_Handle->pI2C, pI2C_Handle->DevAddr, 1);
        }
    }

    if (isITEVTEN && ((pI2C_Handle->pI2C->SR1 >> I2C_SR1_ADDR) && 1)) {
        /* clear ADDR flag */
        I2C_ClearADDRFlag(pI2C_Handle);
    }

    if (isITEVTEN && ((pI2C_Handle->pI2C->SR1 >> I2C_SR1_BTF) && 1)) {
        if (pI2C_Handle->TxRxState == I2C_TX_BUSY) {
            /* make sure TXE is set */
            if (I2C_SR_GetFlag(pI2C_Handle->pI2C, I2C_SR1_TXE)) {
                /* Check Tx len */
                if (pI2C_Handle->TxLen == 0) {
                    /* Check repeated start */
                    if (pI2C_Handle->Sr == 0) {
                        /* Generate STOP condition */
                        I2C_GenerateStopCondition(pI2C_Handle->pI2C);
                    }

                    /* reset all elements in i2c handle stucture */
                    I2C_CloseTransmittion(pI2C_Handle);

                    /* Application callback */
                    I2C_ApplicationEventCallback(pI2C_Handle, I2C_EVENT_BTF);

                }
            }
        }
        else if (pI2C_Handle->TxRxState == I2C_RX_BUSY){ ; }
    }

    if (isITEVTEN && ((pI2C_Handle->pI2C->SR1 >> I2C_SR1_STOPF) && 1)) {
        /* Clear STOPF, read SR1 + Write CR1 */
        pI2C_Handle->pI2C->CR1 |= 0b0;
        
        /* Application callback */
    }

    if (isITEVTEN && isITBUFEN && ((pI2C_Handle->pI2C->SR1 >> I2C_SR1_TXE) && 1)) {
        /* Data transmittion */

        /* Check if device is in master mode */
        if ((pI2C_Handle->pI2C->SR2 >> I2C_SR2_MSL) && 1) {
            if (pI2C_Handle->TxRxState == I2C_TX_BUSY) {
                if (pI2C_Handle->TxLen > 0) {
                    /* Load data into DR */
                    pI2C_Handle->pI2C->DR = *pI2C_Handle->pTxBuffer;

                    pI2C_Handle->TxLen--;

                    pI2C_Handle->pTxBuffer++;
                }
            }
        }
    }
    if (isITEVTEN && isITBUFEN && ((pI2C_Handle->pI2C->SR1 >> I2C_SR1_RXNE) && 1)) {
        /* check device mode */
        if ((pI2C_Handle->pI2C->SR2) && 0b1) {
            /* master mode */
            if (pI2C_Handle->TxRxState == I2C_RX_BUSY) {
                if (pI2C_Handle->RxLen == 1) {
                    *(pI2C_Handle->pRxBuffer) = pI2C_Handle->pI2C->DR;
                    pI2C_Handle->RxLen--;
                }
                else if (pI2C_Handle->RxLen > 1) {
                    if (pI2C_Handle->RxLen == 2) {
                        /* disable ACK */
                        I2C_ManageACK(pI2C_Handle->pI2C, 0);
                    }
                    *(pI2C_Handle->pRxBuffer) = pI2C_Handle->pI2C->DR;
                    pI2C_Handle->RxLen--;
                    rx_count--;
                    pI2C_Handle->pRxBuffer++;
                }
                else if (rx_count == 0){
                    if (pI2C_Handle->Sr == 0) {
                        I2C_GenerateStopCondition(pI2C_Handle->pI2C);
                    }

                    /* Close connection */
                    I2C_CloseReception(pI2C_Handle);

                    /* callback */
                    I2C_ApplicationEventCallback(pI2C_Handle, I2C_EVENT_RX_CMPLT);
                }
            }
        }
    }
}

void I2C_ER_IRQHandling(I2C_Handle_Type *pI2CHandle)
{

	uint32_t temp1,temp2;

    //Know the status of  ITERREN control bit in the CR2
	temp2 = (pI2CHandle->pI2C->CR2) & ( 1 << I2C_CR2_ITERREN);


	temp1 = (pI2CHandle->pI2C->SR1) & ( 1<< I2C_SR1_BERR);
	if(temp1  && temp2 )
	{
		//This is Bus error
		
		//Implement the code to clear the buss error flag 
		pI2CHandle->pI2C->SR1 &= ~( 1 << I2C_SR1_BERR);
		
		//Implement the code to notify the application about the error 
	   I2C_ApplicationEventCallback(pI2CHandle,I2C_ERROR_BERR);
	}

	temp1 = (pI2CHandle->pI2C->SR1) & ( 1 << I2C_SR1_ARLO );
	if(temp1  && temp2)
	{
		//This is arbitration lost error
		
		//Implement the code to clear the arbitration lost error flag
		
		//Implement the code to notify the application about the error 
	   
	}

	temp1 = (pI2CHandle->pI2C->SR1) & ( 1 << I2C_SR1_AF);
	if(temp1  && temp2)
	{
		//This is ACK failure error
		
	    //Implement the code to clear the ACK failure error flag
		
		//Implement the code to notify the application about the error 
        I2C_ApplicationEventCallback(pI2CHandle, I2C_ERROR_AF);
	}

	temp1 = (pI2CHandle->pI2C->SR1) & ( 1 << I2C_SR1_OVR);
	if(temp1  && temp2)
	{
		//This is Overrun/underrun
		
	    //Implement the code to clear the Overrun/underrun error flag
		
		//Implement the code to notify the application about the error 
	}

	temp1 = (pI2CHandle->pI2C->SR1) & ( 1 << I2C_SR1_TIMEOUT);
	if(temp1  && temp2)
	{
		//This is Time out error
		
	    //Implement the code to clear the Time out error flag
		
		//Implement the code to notify the application about the error 
	}

}

void I2C_ApplicationEventCallback(I2C_Handle_Type* pI2CHandle, uint8_t AppEvt){
    switch (AppEvt) {
        case I2C_ERROR_AF:
            /* Slave send NACK, close transmittion */
            I2C_CloseTransmittion(pI2CHandle);

            /* Generate STOP condition */
            I2C_GenerateStopCondition(pI2CHandle->pI2C);

            /* Infinite loop */
            while (1) {
                GPIOA->ODR ^= (1 << 5);
                delay_ms(1000);
            }
            break;
    }
}
