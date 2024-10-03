#ifndef STM32G4XX_I2C_DRIVERS_H
#define STM32G4XX_I2C_DRIVERS_H

/* Bit postion of I2C registers */
#include "stm32g431xx.h"
#include <stdint.h>


/* Enable clock peripharals */
#define I2C1_PCLK_EN()          RCC->APB1ENR1 |= 1 << 21
#define I2C2_PCLK_EN()          RCC->APB1ENR1 |= 1 << 22
#define I2C3_PCLK_EN()          RCC->APB1ENR1 |= 1 << 30

/* Bit position of I2C register */

#define I2C_SR1_SB              0    // Start bit (Master mode)
#define I2C_SR1_ADDR            1    // Address sent (Master) / matched (Slave)
#define I2C_SR1_BTF             2    // Byte transfer finished
#define I2C_SR1_ADD10           3    // 10-bit header sent (Master mode)
#define I2C_SR1_STOPF           4    // Stop detection (Slave mode)
#define I2C_SR1_RXNE            6    // Data register not empty (Receiver)
#define I2C_SR1_TXE             7    // Data register empty (Transmitter)
#define I2C_SR1_BERR            8    // Bus error
#define I2C_SR1_ARLO            9    // Arbitration lost (Master mode)
#define I2C_SR1_AF              10   // Acknowledge failure
#define I2C_SR1_OVR             11   // Overrun/Underrun
#define I2C_SR1_PECERR          12   // PEC error in reception
#define I2C_SR1_TIMEOUT         14   // Timeout or Tlow error
#define I2C_SR1_SMBALERT        15   // SMBus alert

#define I2C_SR2_MSL             0    // Master/slave
#define I2C_SR2_BUSY            1    // Bus busy
#define I2C_SR2_TRA             2    // Transmitter/receiver
#define I2C_SR2_GENCALL         4    // General call address (Slave mode)
#define I2C_SR2_SMBDEFAULT      5    // SMBus device default address (Slave mode)
#define I2C_SR2_SMBHOST         6    // SMBus host header (Slave mode)
#define I2C_SR2_DUALF           7    // Dual flag (Slave mode)
#define I2C_SR2_PEC             8    // Packet error checking register

#define I2C_CCR_CCR             0    // Clock control register in Fast/Standard mode (12 bits)
#define I2C_CCR_DUTY            14   // Fast mode duty cycle
#define I2C_CCR_FS              15   // I2C master mode selection


/* I2C user configuration macros */
#define I2C_SCL_SPEED_STANDARD  100000
#define I2C_SCL_SPEED_FAST4K    400000
#define I2C_SCL_SPEED_FAST2K    200000

#define I2C_ACK_ENABLE          1
#define I2C_ACK_DISABLE         0

#define I2C_FM_DUTY_2           0
#define I2C_FM_DUTY_16_9        1


typedef struct {
    uint32_t    I2C_SCLSpeed;
    uint8_t     I2C_Device_Address;
    uint8_t     I2C_ACK_Control;
    uint16_t    I2C_FMDutyCycle;
} I2C_Config_Type; 

typedef struct {
    I2C_TypeDef* pI2C;
    I2C_Config_Type I2C_Config;
} I2C_Handle_Type;

void I2C_Init(I2C_Handle_Type* pI2C_Handle);
void I2C_Clock_Control(I2C_TypeDef* pI2C, uint8_t EnOrDi);

void I2C_Sendata(I2C_TypeDef* pI2C, uint8_t* pTxBuffer, uint32_t Len);
void I2C_RecieveData(I2C_TypeDef* pI2C, uint8_t* pRxBuffer, uint32_t Len);

#endif /* end of include guard: STM32G4XX_I2C_DRIVERS_H */
