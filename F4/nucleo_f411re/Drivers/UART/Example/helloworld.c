#include "stm32f411xx.h"
#include "stm32f4xx_uart_driver.h"
#include "stm32f411xx_clock.h"
#include "bsp.h"
#include <stdint.h>
#include <string.h>

USART_Handle_Type usart1_handle;

void USART_Config(void){
    USART_PeripheralControl(USART1, 1);

    /* Initialize USART */
    usart1_handle.pUSART = USART1;
    usart1_handle.USART_Config.USART_Mode = USART_MODE_TXRX;
    usart1_handle.USART_Config.USART_Baud = 9600;
    usart1_handle.USART_Config.USART_NoOfStopBits = USART_STOPBITS_1;
    usart1_handle.USART_Config.USART_WordLength = USART_WORDLEN_8BITS;

    USART_Init(&usart1_handle);
}

void GPIO_Config(void){
    GPIOA_PCLK_EN();
    GPIOC_PCLK_EN();

    GPIOA->MODER |= (1 << (LED_PIN * 2));

    /* USART1 PA9:TX PA10:RX */

    /* PA2, PA3 as alternative function */
    GPIOA->MODER |= (2 << 18);
    GPIOA->MODER |= (2 << 20);

    /* Set AF07 to PA9, PA10 */
    GPIOA->AFR[1] |= (0x77 << 4);

    /* Push-pull */
    GPIOA->OTYPER &= ~(0x3 << 9);

    /* High speed */
    GPIOA->OSPEEDR |= (0xF << (9 * 2));

    /* Pull-up ON */
    GPIOA->PUPDR &= ~(0xF << (9 * 2));
    // GPIOA->PUPDR |=  (0x5 << (9 * 2));
}

int main(){
    USART_Config();
    usart1_handle.pUSART->SR &= ~(1 << USART_SR_TC);

    GPIO_Config();

    // char* thechar = "hello world";
    
    uint8_t pRxBuffer = 0;

    for (;;) {
        USART_ReceiveData(&usart1_handle, &pRxBuffer, 1);

        if (pRxBuffer == '2') {
            LED_OFF();
        }
        if (pRxBuffer == '3') {
            LED_ON();
        }

        delay_ms(1);
    }
}
