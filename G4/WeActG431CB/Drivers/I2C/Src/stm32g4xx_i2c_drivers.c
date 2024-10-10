#include "stm32g4xx_i2c_drivers.h"
#include "bsp.h"
#include "stm32g431xx.h"
#include <stdint.h>

void I2C_Clock_Control(I2C_TypeDef *pI2C, uint8_t EnOrDi){
    if (EnOrDi) {
        if      (pI2C == I2C1) { I2C1_PCLK_EN(); }
        else if (pI2C == I2C2) { I2C2_PCLK_EN(); }
        else if (pI2C == I2C3) { I2C3_PCLK_EN(); }

        pI2C->CR1 |= (1 << I2C_CR1_PE_Pos);
    }
}

void I2C_Init2(I2C_Handle_Type* pI2C_Handle){
    uint32_t temp = 0;

    temp |= pI2C_Handle->I2C_Config.I2C_NACK_Control << I2C_CR2_NACK_Pos;

    /* 16mhz -> 100kbps, 16/9 */
    I2C1->TIMINGR = (7 << 28) |  // PRESC = 7 (Divide APB1 clock by 8)
                    (1 << 20) |  // SCLDEL = 1
                    (1 << 16) |  // SDADEL = 1
                    (64 << 8) |  // SCLL = 64 (Low period)
                    (36 << 0);   // SCLH = 36 (High period)

    pI2C_Handle->pI2C->CR2 = temp;

    temp = 0;
    temp |= pI2C_Handle->I2C_Config.I2C_Device_Address << 1;
    pI2C_Handle->pI2C->OAR1 |= temp;
}

void I2C_Init(I2C_Handle_Type* pI2C_Handle){
    uint32_t tmp = 0;

    I2C_Clock_Control(pI2C_Handle->pI2C, 1);

    /**
     * I2C_CR2_NACK: NACK generation (slave mode)
     * 0: an ACK is sent after current received byte.
     * 1: a NACK is sent after current received byte.
     */
    tmp |= pI2C_Handle->I2C_Config.I2C_NACK_Control << I2C_CR2_NACK_Pos;
    pI2C_Handle->pI2C->CR2 &= ~(1 << I2C_CR2_NACK_Pos);
    pI2C_Handle->pI2C->CR2 |= tmp;

    /* FREQ */
    if (pI2C_Handle->I2C_Config.I2C_SCLSpeed == I2C_SCL_SPEED_STANDARD) {
        /* 16mhz -> 100kbps */
        I2C1->TIMINGR =     (0 << 28) |  // PRESC
                            (15 << 20) | // SCLDEL
                            (15 << 16) | // SDADEL
                            (63 << 8)  | // SCLH
                            (74 << 0);   // SCLL
    }
    else if (pI2C_Handle->I2C_Config.I2C_SCLSpeed == I2C_SCL_SPEED_FAST4K){

        if (pI2C_Handle->I2C_Config.I2C_FMDutyCycle == I2C_FM_DUTY_16_9) {
            I2C1->TIMINGR = (0 << 28) |  // PRESC (No prescaler)
                            (7 << 20)  | // SCLDEL (0.5 µs hold time)
                            (3 << 16)  | // SDADEL (0.25 µs setup time)
                            (13 << 8)  | // SCLH (High time 0.9 µs)
                            (25 << 0);   // SCLL (Low time 1.6 µs)

        }
    }
    

    /* Device address */
    tmp = pI2C_Handle->I2C_Config.I2C_Device_Address << 1;
    pI2C_Handle->pI2C->OAR1 &= ~(0x7F << 1);
    pI2C_Handle->pI2C->OAR1 |= tmp;
    pI2C_Handle->pI2C->OAR1 |= (1 << I2C_OAR1_OA1EN_Pos);
}

uint8_t I2C_SR_GetFlag(I2C_TypeDef* pI2C, uint32_t Flag){
    if ((pI2C->ISR >> Flag) & 1){
        return 1;
    }
    return 0;
}


void I2C_ExecuteAddressPhase(I2C_TypeDef* pI2C, int8_t SlaveAddress){
    SlaveAddress <<= 1;    
    SlaveAddress &= ~1;
    pI2C->TXDR = SlaveAddress;
}

static void I2C_GenerateStartCondition(I2C_TypeDef* pI2C){
    /* Generate start */
    pI2C->CR2 |= (1 << I2C_CR2_START_Pos);
}

static void I2C_GenerateStopCondition(I2C_TypeDef* pI2C){
    /* Generate start */
    pI2C->CR2 |= (1 << I2C_CR2_STOP_Pos);
}

void I2C_ClearADDRFlag(I2C_TypeDef* pI2C){
    pI2C->ICR |= 1 << I2C_ICR_ADDRCF_Pos;
}

void I2C_MasterSendata(I2C_Handle_Type* pI2C_Handle, uint8_t* pTxBuffer, uint32_t Len, uint8_t SlaveAddress){
    /* Start */
    I2C_GenerateStartCondition(pI2C_Handle->pI2C);

    /* Confirm start condition */
    while (I2C_SR_GetFlag(pI2C_Handle->pI2C, I2C_CR2_START_Pos));

    /* Send address and w/r bit */
    I2C_ExecuteAddressPhase(pI2C_Handle->pI2C, pI2C_Handle->I2C_Config.I2C_Device_Address);

    /* Confirm  */
    while (!I2C_SR_GetFlag(pI2C_Handle->pI2C, I2C_ISR_TXE_Pos));

    /* Clear ADDR flag */
    I2C_ClearADDRFlag(pI2C_Handle->pI2C);

    /* Send bytes 'til len = 0 */
    while (Len > 0) {
        while (!I2C_SR_GetFlag(pI2C_Handle->pI2C, I2C_ISR_TXE_Pos));
        pI2C_Handle->pI2C->TXDR = *pTxBuffer;
        pTxBuffer++;
        Len--;
    }

    /* Check TXE, BTF flags before generate STOP condition */
    while (!I2C_SR_GetFlag(pI2C_Handle->pI2C, I2C_ISR_TXE_Pos));
    while (!I2C_SR_GetFlag(pI2C_Handle->pI2C, I2C_ISR_TC_Pos));

    /* Generate STOP condition */
    I2C_GenerateStopCondition(pI2C_Handle->pI2C);
}

void I2C_SlaveReceiveData(I2C_Handle_Type* pI2C_Handle, uint8_t* pRxBuffer, uint8_t Len) {
    // Wait until a data request is received
    while (!I2C_SR_GetFlag(pI2C_Handle->pI2C, I2C_ISR_ADDR_Pos));


    // Clear the ADDR flag
    I2C_ClearADDRFlag(pI2C_Handle->pI2C);

    // Read the received data
    while (Len > 0) {
        // Wait for data to be received
        while (!I2C_SR_GetFlag(pI2C_Handle->pI2C, I2C_ISR_RXNE_Pos));

        *pRxBuffer = pI2C_Handle->pI2C->RXDR;
        pRxBuffer++;
        Len--;
    }

    /* Clear stop flag */
    // pI2C_Handle->pI2C->ICR |= (1 << I2C_ICR_STOPCF_Pos);
}
void I2C_SlaveSendata(I2C_Handle_Type* pI2C_Handle, uint8_t* pTxBuffer, uint32_t Len){
    /* Wait for ADDR */
    while(!I2C_SR_GetFlag(pI2C_Handle->pI2C, I2C_ISR_ADDR_Pos));

    /* Clear the ADDR flag */
    I2C_ClearADDRFlag(pI2C_Handle->pI2C);
    

    LED_TOGGLE();

    while(Len > 0)
    {
        while (!I2C_SR_GetFlag(pI2C_Handle->pI2C, I2C_ISR_TXIS_Pos));
        pI2C_Handle->pI2C->TXDR = *pTxBuffer;
        pTxBuffer++;
        Len--;
    }

    /* Wait for STOP condition */
    while (!I2C_SR_GetFlag(pI2C_Handle->pI2C, I2C_ISR_STOPF_Pos)); 
    /* Clear STOP flag */
    I2C2->ICR |= I2C_ICR_STOPCF;          

    LED_TOGGLE();
}
