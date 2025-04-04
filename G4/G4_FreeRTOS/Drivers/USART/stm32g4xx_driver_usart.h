#ifndef STM32G4XX_USART_DRIVERS_H
#define STM32G4XX_USART_DRIVERS_H

#include "stm32g4xx.h"
#include <stdint.h>

#define USART_BAUDRATE_9600 (uint8_t)0x0683

#define USART_TX_ONLY       1
#define USART_RX_ONLY       2
#define USART_RXTX          3

#define USART_WORDLEN_8     0
#define USART_WORDLEN_9     1
#define USART_WORDLEN_7     2

#define USART_Parity_Even   0
#define USART_Parity_Odd    1
#define USART_Parity_None   2

#define USART_STOPBITS_1    0
#define USART_STOPBITS_0_5  1 
#define USART_STOPBITS_2    2
#define USART_STOPBITS_1_5  3

typedef struct{
    uint8_t Baudrate;
    uint8_t Mode;
    uint8_t StopBits;
    uint8_t Wordlen;
    uint8_t Parity;
} USART_Config_Type;

typedef struct{
    USART_TypeDef* pUSART;
    USART_Config_Type USART_Config;
} USART_Handle_Type;

void USART_Init(USART_Handle_Type* pUSART_Handle);
void USART_SendData(USART_Handle_Type* pUSART_Handle, uint8_t* TxBuffer, uint8_t Len);

#endif /* end of include guard: STM32G4XX_USART_DRIVERS_H */
