#include "stm32f411xx_i2c_driver.h"
#include "stm32f411xx.h"
#include "stm32f411xx_clock.h"
#include "stm32f411xx_types.h"
#include <stddef.h>
#include <stdint.h>

void I2C_Clock_Control(I2C_Type* pI2C, uint8_t EnOrDi){
    if (EnOrDi) {
        if      (pI2C == I2C1) { RCC->APB1ENR |= 1 << 21; }
        else if (pI2C == I2C2) { RCC->APB1ENR |= 1 << 22; }
        else if (pI2C == I2C3) { RCC->APB1ENR |= 1 << 23; }
    }
    else {
        if      (pI2C == I2C1) { RCC->APB1ENR &= ~(1 << 21); }
        else if (pI2C == I2C2) { RCC->APB1ENR &= ~(1 << 22); }
        else if (pI2C == I2C3) { RCC->APB1ENR &= ~(1 << 23); }
    }
}

void I2C_Init(I2C_Handle_Type* pI2C_Handle){
    uint16_t tmp = 0;

    // Ensure the WWDG is disabled if you don't intend to use it.
    RCC->APB1ENR &= ~(1 << 11);  // Disable the WWDG clock

    /* Enable peripharel clock */
    I2C_Clock_Control(pI2C_Handle->pI2C, 1);

    /* ACK */
    tmp |= (pI2C_Handle->I2C_Config.I2C_ACK_Control) << I2C_CR1_ACK;
    pI2C_Handle->pI2C->CR1 &= ~(1 << I2C_CR1_ACK);
    pI2C_Handle->pI2C->CR1 |= tmp;

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


uint8_t I2C_SR_GetFlag(I2C_Type* pI2C, uint32_t Flag){
    if ((pI2C->SR1 >> Flag) & 1){
        return 1;
    }
    return 0;
}


void I2C_ExecuteAddressPhase(I2C_Type* pI2C, uint8_t SlaveAddress, uint8_t isRead){
    uint8_t temp = 0x3;    
    temp <<= 1;

    /* Read 1 | write 0 */
    if (isRead) { temp |= 1; }

    pI2C->DR = temp;
}

static void I2C_GenerateStartCondition(I2C_Type* pI2C){
    /* Generate start */
    pI2C->CR1 |= (1 << I2C_CR1_START);
}

static void I2C_GenerateStopCondition(I2C_Type* pI2C){
    /* Generate start */
    pI2C->CR1 |= (1 << I2C_CR1_STOP);
}

void I2C_ClearADDRFlag(I2C_Type* pI2C){
    uint32_t dummy1 = pI2C->SR1;
    dummy1 = pI2C->SR2;
    (void)dummy1;
}

void I2C_ManageACK(I2C_Type* pI2C, uint8_t EnOrDi){
    if (EnOrDi == 1) {
        pI2C->CR1 |= (1 << I2C_CR1_ACK);
    }
    else{
        pI2C->CR1 &= ~(1 << I2C_CR1_ACK);
    }
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
    I2C_ClearADDRFlag(pI2C_Handle->pI2C);

    /* Send bytes 'til len = 0 */
    while (Len > 0) {
        while (!I2C_SR_GetFlag(pI2C_Handle->pI2C, I2C_SR1_TXE));
        pI2C_Handle->pI2C->DR = *pTxBuffer;
        pTxBuffer++;
        Len--;
    }

    /* Check TXE, BTF flags before generate STOP condition */
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

    /* Confirm  */
    while (!I2C_SR_GetFlag(pI2C_Handle->pI2C, I2C_SR1_ADDR));

    if (Len == 1) {
        /* Generate NACK to stop transmission */
        I2C_ManageACK(pI2C_Handle->pI2C, 0);

        /* Clear ADDR flag */
        I2C_ClearADDRFlag(pI2C_Handle->pI2C);

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
        I2C_ClearADDRFlag(pI2C_Handle->pI2C);

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
