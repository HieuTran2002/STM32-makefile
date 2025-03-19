#ifndef STM32G0_USART_H
#define STM32G0_USART_H

#include "stdint.h"
#include "stdbool.h"
#include "stm32g031xx.h"

typedef struct {
    uint32_t baudrate;
    uint8_t wordlen;
    uint8_t stopBits;
    bool parityEnable;
    uint8_t parityMode;
    uint8_t interrupt;
} USART_Config_Tyep;

typedef struct {
    USART_TypeDef* pUSART;
    USART_Config_Tyep config;
} USART_Handle_Type;

#endif /* end of include guard: STM32G0_USART_H */

void USART_Init(USART_Handle_Type* USART_Handle);
void USART_SendChar(USART_TypeDef* USART, uint8_t ch);
void USART_SendString(USART_TypeDef* USART, const char* str);
uint8_t USART_ReceiveChar(USART_TypeDef* USART);
bool USART_IsDataAvailable(void);
void handle_received_string(char* str);
