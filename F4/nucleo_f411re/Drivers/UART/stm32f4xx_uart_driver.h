#ifndef STM32F4XX_UART_DRIVER_H
#define STM32F4XX_UART_DRIVER_H

#include <stdint.h>
#include "stm32f411xx_types.h"

// USART Status Register (USART_SR) bit definitions
#define USART_SR_PE      0   // Parity Error
#define USART_SR_FE      1   // Framing Error
#define USART_SR_NE      2   // Noise Error
#define USART_SR_ORE     3   // Overrun Error
#define USART_SR_IDLE    4   // IDLE Line Detected
#define USART_SR_RXNE    5   // Read Data Register Not Empty
#define USART_SR_TC      6   // Transmission Complete
#define USART_SR_TXE     7   // Transmit Data Register Empty
#define USART_SR_LBD     8   // LIN Break Detection Flag
#define USART_SR_CTS     9   // CTS Flag

// USART Data Register (USART_DR) bit definitions
#define USART_DR_DR      0   // Data value (9 bits)

// USART Baud Rate Register (USART_BRR) bit definitions
#define USART_BRR_DIV_FRACTION  0   // Fraction of USARTDIV (4 bits)
#define USART_BRR_DIV_MANTISSA  4   // Mantissa of USARTDIV (12 bits)

// USART Control Register 1 (USART_CR1) bit definitions
#define USART_CR1_SBK     0   // Send Break
#define USART_CR1_RWU     1   // Receiver Wakeup
#define USART_CR1_RE      2   // Receiver Enable
#define USART_CR1_TE      3   // Transmitter Enable
#define USART_CR1_IDLEIE  4   // IDLE Interrupt Enable
#define USART_CR1_RXNEIE  5   // RXNE Interrupt Enable
#define USART_CR1_TCIE    6   // Transmission Complete Interrupt Enable
#define USART_CR1_TXEIE   7   // TXE Interrupt Enable
#define USART_CR1_PEIE    8   // Parity Error Interrupt Enable
#define USART_CR1_PS      9   // Parity Selection
#define USART_CR1_PCE     10  // Parity Control Enable
#define USART_CR1_WAKE    11  // Wakeup Method
#define USART_CR1_M       12  // Word Length
#define USART_CR1_UE      13  // USART Enable
#define USART_CR1_OVER8   15  // Oversampling Mode

// USART Control Register 2 (USART_CR2) bit definitions
#define USART_CR2_ADD     0   // Address of the USART node
#define USART_CR2_LBDL    5   // LIN Break Detection Length
#define USART_CR2_LBDIE   6   // LIN Break Detection Interrupt Enable
#define USART_CR2_LBCL    8   // Last Bit Clock Pulse
#define USART_CR2_CPHA    9   // Clock Phase
#define USART_CR2_CPOL    10  // Clock Polarity
#define USART_CR2_CLKEN   11  // Clock Enable
#define USART_CR2_STOP    12  // STOP bits (2 bits)
#define USART_CR2_LINEN   14  // LIN Mode Enable

// USART Control Register 3 (USART_CR3) bit definitions
#define USART_CR3_EIE     0   // Error Interrupt Enable
#define USART_CR3_IREN    1   // IrDA Mode Enable
#define USART_CR3_IRLP    2   // IrDA Low-Power
#define USART_CR3_HDSEL   3   // Half-Duplex Selection
#define USART_CR3_NACK    4   // Smartcard NACK Enable
#define USART_CR3_SCEN    5   // Smartcard Mode Enable
#define USART_CR3_DMAR    6   // DMA Enable Receiver
#define USART_CR3_DMAT    7   // DMA Enable Transmitter
#define USART_CR3_RTSE    8   // RTS Enable
#define USART_CR3_CTSE    9   // CTS Enable
#define USART_CR3_CTSIE   10  // CTS Interrupt Enable
#define USART_CR3_ONEBIT  11  // One sample bit method enable

// USART Guard time and prescaler register (USART_GTPR) bit definitions
#define USART_GTPR_PSC    0   // Prescaler value (8 bits)
#define USART_GTPR_GT     8   // Guard time value (8 bits)

#define USART_WORDLEN_8BITS             0
#define USART_WORDLEN_9BITS             1
#define USART_STOPBITS_1                0
#define USART_STOPBITS_2                2
#define USART_PS_EVEN                   0
#define USART_PS_ODD                    1

#define USART_MODE_ONLY_RX              0
#define USART_MODE_ONLY_TX              1
#define USART_MODE_TXRX                 2

#define USART_HW_FLOW_CTRL_CTS          0
#define USART_HW_FLOW_CTRL_RTS          1
#define USART_HW_FLOW_CTRL_CTS_RTS      2

typedef struct {
    uint8_t USART_Mode;
    uint32_t USART_Baud;
    uint8_t USART_NoOfStopBits;
    uint8_t USART_WordLength;
    uint8_t USART_ParityControl;
    uint8_t USART_HWFlowControl;
} USART_Config_Type;

typedef struct{
    USART_TypeDef* pUSART;    
    USART_Config_Type USART_Config;
} USART_Handle_Type;

void USART_PeriClockControl(USART_TypeDef* pUSART, uint8_t EnOrDi);
void USART_PeripheralControl(USART_TypeDef* pUSART, uint8_t EnOrDi);

uint8_t USART_GetFlag(USART_TypeDef* pUSART, uint8_t StatusFlagName);
void USART_ClearFlag(USART_TypeDef* pUSART, uint8_t StatusFlagName);

void USART_IRQInterruptConfig(uint8_t IRQNumber, uint8_t EnOrDi);
void USART_IRQPriorityConfig(uint8_t IRQNumber, uint8_t IRQPriority);

#endif
