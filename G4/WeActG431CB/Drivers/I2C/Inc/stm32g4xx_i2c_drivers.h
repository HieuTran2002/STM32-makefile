#ifndef STM32G4XX_I2C_DRIVERS_H
#define STM32G4XX_I2C_DRIVERS_H

/* Bit postion of I2C registers */
#include "stm32g431xx.h"
#include <stdint.h>


/* Enable clock peripharals */
#define I2C1_PCLK_EN()          (RCC->APB1ENR1 |= 1 << 21)
#define I2C2_PCLK_EN()          (RCC->APB1ENR1 |= 1 << 22)
#define I2C3_PCLK_EN()          (RCC->APB1ENR1 |= 1 << 30)

/* Bit position of I2C register */


/* I2C user configuration macros */
#define I2C_SCL_SPEED_STANDARD  100000
#define I2C_SCL_SPEED_FAST4K    400000
#define I2C_SCL_SPEED_FAST2K    200000

#define I2C_RESPONE_ACK         0
#define I2C_RESPONE_NACK        1

#define I2C_FM_DUTY_2           0
#define I2C_FM_DUTY_16_9        1


typedef struct {
    uint32_t    I2C_SCLSpeed;
    uint8_t     I2C_Device_Address;
    uint8_t     I2C_NACK_Control;
    uint16_t    I2C_FMDutyCycle;
} I2C_Config_Type; 

typedef struct {
    I2C_TypeDef* pI2C;
    I2C_Config_Type I2C_Config;
} I2C_Handle_Type;

void I2C_Init(I2C_Handle_Type* pI2C_Handle);
void I2C_Clock_Control(I2C_TypeDef* pI2C, uint8_t EnOrDi);

uint8_t I2C_SR_GetFlag(I2C_TypeDef* pI2C, uint32_t Flag);
void I2C_Init(I2C_Handle_Type* pI2C_Handle);
void I2C_Clock_Control(I2C_TypeDef* pI2C, uint8_t EnOrDi);

void I2C_MasterSendata(I2C_Handle_Type* pI2C_Handle, uint8_t* pTxBuffer, uint32_t Len, uint8_t SlaveAddress);
void I2C_SlaveReceiveData(I2C_Handle_Type* pI2C_Handle, uint8_t* pRxBuffer, uint8_t Len);

void I2C_SlaveSendata(I2C_Handle_Type* pI2C_Handle, uint8_t* pTxBuffer, uint32_t Len);

void I2C_ClearADDRFlag(I2C_TypeDef* pI2C);

#endif /* end of include guard: STM32G4XX_I2C_DRIVERS_H */
