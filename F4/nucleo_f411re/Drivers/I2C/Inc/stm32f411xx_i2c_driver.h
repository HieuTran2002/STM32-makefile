#ifndef STM32F411XX_I2C_DRIVERS_H
#define STM32F411XX_I2C_DRIVERS_H

#include "stm32f411xx.h"

/* Bit position of I2C register */
#define I2C_CR1_PE           0  // Peripheral Enable
#define I2C_CR1_SMBUS        1  // SMBus Mode
#define I2C_CR1_SMBTYPE      3  // SMBus Type
#define I2C_CR1_ENARP        4  // ARP Enable
#define I2C_CR1_ENPEC        5  // PEC Enable
#define I2C_CR1_ENGC         6  // General Call Enable
#define I2C_CR1_NOSTRETCH    7  // Clock Stretching Disable (Slave mode)
#define I2C_CR1_START        8  // Start Generation
#define I2C_CR1_STOP         9  // Stop Generation
#define I2C_CR1_ACK          10 // Acknowledge Enable
#define I2C_CR1_POS          11 // Acknowledge/PEC Position (for data reception)
#define I2C_CR1_PEC          12 // Packet Error Checking
#define I2C_CR1_ALERT        13 // SMBus Alert
#define I2C_CR1_SWRST        15 // Software Reset

#define I2C_CR2_FREQ         0  // Frequency bits [5:0]
#define I2C_CR2_ITERREN      8  // Error Interrupt Enable
#define I2C_CR2_ITEVTEN      9  // Event Interrupt Enable
#define I2C_CR2_ITBUFEN      10 // Buffer Interrupt Enable
#define I2C_CR2_DMAEN        11 // DMA Requests Enable
#define I2C_CR2_LAST         12 // DMA Last Transfer

#define I2C_SR1_SB           0  // Start Bit (Master mode)
#define I2C_SR1_ADDR         1  // Address Sent/Matched (Master/Slave)
#define I2C_SR1_BTF          2  // Byte Transfer Finished
#define I2C_SR1_ADD10        3  // 10-bit Header Sent (Master mode)
#define I2C_SR1_STOPF        4  // Stop Detection (Slave mode)
#define I2C_SR1_RXNE         6  // Data Register not Empty (Receive)
#define I2C_SR1_TXE          7  // Data Register Empty (Transmit)
#define I2C_SR1_BERR         8  // Bus Error
#define I2C_SR1_ARLO         9  // Arbitration Lost (Master mode)
#define I2C_SR1_AF           10 // Acknowledge Failure
#define I2C_SR1_OVR          11 // Overrun/Underrun
#define I2C_SR1_PECERR       12 // PEC Error in Reception
#define I2C_SR1_TIMEOUT      14 // Timeout or Tlow Error
#define I2C_SR1_SMBALERT     15 // SMBus Alert

#define I2C_SR2_MSL          0  // Master/Slave
#define I2C_SR2_BUSY         1  // Bus Busy
#define I2C_SR2_TRA          2  // Transmitter/Receiver
#define I2C_SR2_GENCALL      4  // General Call Address (Slave mode)
#define I2C_SR2_SMBDEFAULT   5  // SMBus Device Default Address
#define I2C_SR2_SMBHOST      6  // SMBus Host Header
#define I2C_SR2_DUALF        7  // Dual Flag (Slave mode)
#define I2C_SR2_PEC          8  // Packet Error Checking (Slave mode)

#define I2C_CCR_CCR          0  // Clock Control Bits [11:0]
#define I2C_CCR_DUTY         14 // Fast Mode Duty Cycle
#define I2C_CCR_FS           15 // I2C Master Mode Selection

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

#endif /* end of include guard: STM32F411XX_I2C_DRIVERS_H */
