#include "stm32g4xx_i2c_drivers.h"
#include "stm32g431xx.h"
#include <stdint.h>


void I2C_Clock_Control(I2C_TypeDef *pI2C, uint8_t EnOrDi){
    if (EnOrDi) {
        if      (pI2C == I2C1) { I2C1_PCLK_EN(); }
        else if (pI2C == I2C2) { I2C2_PCLK_EN(); }
        else if (pI2C == I2C3) { I2C3_PCLK_EN(); }
    }
}

void I2C_Init(I2C_Handle_Type* pI2C_Handle){
    uint32_t temp = 0;

    temp |= pI2C_Handle->I2C_Config.I2C_ACK_Control << I2C_CR2_NACK_Pos;

    /* 16mhz -> 100kbps */
    I2C1->TIMINGR = (7 << 28) |  // PRESC = 7 (Divide APB1 clock by 8)
                    (1 << 20) |  // SCLDEL = 1
                    (1 << 16) |  // SDADEL = 1
                    (64 << 8) |  // SCLL = 64 (Low period)
                    (36 << 0);   // SCLH = 36 (High period)



    pI2C_Handle->pI2C->CR2 |= temp;

    temp = 0;
    temp |= pI2C_Handle->I2C_Config.I2C_Device_Address << 1;
    pI2C_Handle->pI2C->OAR1 |= temp;

}
