#include "stm32f411xx_i2c_driver.h"
#include <stdint.h>

void I2C_Init(I2C_Handle_Type* pI2C_Handle){
    uint32_t tmp = 0;

    /* ACK */
    tmp |= pI2C_Handle->I2C_Config.I2C_ACK_Control << I2C_CR1_ACK;
    pI2C_Handle->pI2C->CR1 &= ~(1 << I2C_CR1_ACK);
    pI2C_Handle->pI2C->CR1 |= tmp;

    /* FREQ */
    tmp = 16 << I2C_CR2_FREQ;
    pI2C_Handle->pI2C->CCR &= ~(0x3F << I2C_CR2_FREQ);
    pI2C_Handle->pI2C->CCR |= tmp;

    /* Device address */
    tmp = pI2C_Handle->I2C_Config.I2C_Device_Address << 1;
    pI2C_Handle->pI2C->OAR1 &= ~(0x7F << 1);
    pI2C_Handle->pI2C->OAR1 |= tmp;

}

