#include "stm32g0_usart.h"
#include "stm32g031xx.h"

void USART_PeripheralClockControl(USART_TypeDef* USART){
    USART == USART2 ? RCC->APBENR1 |= RCC_APBENR1_USART2EN : 0;
}

void USART_Init(USART_Handle_Type* USART_Handle){
    USART_PeripheralClockControl(USART_Handle->pUSART);

    USART_Handle->pUSART->CR1 = 0;

    // Set baudrate (assuming 16MHz system clock)
    uint32_t usartdiv = (16000000 + (USART_Handle->config.baudrate/2)) / USART_Handle->config.baudrate;
    USART_Handle->pUSART->BRR = usartdiv;

    // config wordle
    if (USART_Handle->config.wordlen == 9) {
        USART_Handle->pUSART->CR1 |= USART_CR1_M0;
    }

    // Configure parity
    if (USART_Handle->config.parityEnable) {
        USART_Handle->pUSART->BRR |= USART_CR1_PCE;
        // Odd parity
        if (USART_Handle->config.parityEnable == 1) {
            USART_Handle->pUSART->CR1 |= USART_CR1_PS;
        }
    }

    // Configure stopbits
    if (USART_Handle->config.stopBits == 2) {
        USART_Handle->pUSART->CR2 |= USART_CR2_STOP_1;
    }

    // Enable both RX and TX
    USART_Handle->pUSART->CR1 |= USART_CR1_TE | USART_CR1_RE;

    if (USART_Handle->config.interrupt){
        USART_Handle->pUSART->CR1 |= USART_CR1_RXNEIE_RXFNEIE;
        NVIC_SetPriority(USART2_IRQn, 2);
        __NVIC_EnableIRQ(USART2_IRQn);
    }

    // Enable USART
    USART_Handle->pUSART->CR1 |= USART_CR1_UE;
}

void USART_SendChar(USART_TypeDef* USART, uint8_t ch){
    while (!(USART->ISR & USART_ISR_TXE_TXFNF));
    USART->TDR = ch;
}

void USART_SendString(USART_TypeDef* USART, const char* str){
    while(*str) {
        USART_SendChar(USART, *str++);
    }
}

uint8_t USART_ReceiveChar(USART_TypeDef* USART) {
    while (!(USART->ISR & USART_ISR_RXNE_RXFNE));
    return (uint8_t)USART->RDR;
}

void USART2_IRQHandler(void){
    if (USART2->ISR & USART_ISR_RXNE_RXFNE) {
        GPIOA->ODR ^= (1 << 4);
        (void)USART2->RDR;
    }
        // Handle errors if needed
    if (USART2->ISR & (USART_ISR_PE | USART_ISR_FE | USART_ISR_NE | USART_ISR_ORE)) {
        // Clear error flags
        USART2->ICR = USART_ICR_PECF | USART_ICR_FECF | USART_ICR_CMCF | USART_ICR_ORECF;
    }
}
