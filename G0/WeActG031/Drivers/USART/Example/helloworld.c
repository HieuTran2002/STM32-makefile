#include "stm32g031_clock.h"
#include "stm32g031_gpio_driver.h"
#include "stm32g0_usart.h"
#include <stdint.h>

USART_Handle_Type USART_Handle;

void GPIO_Config (){
    GPIO_Handle_Type GPIO_Handle = {
        .GPIO_Config.AltFunc = 1,
        .GPIO_Config.PinMode = GPIO_MODER_ALTFNC,
        .GPIO_Config.PinNumber = 2,
        .pGPIO = GPIOA
    };
    GPIO_Init(&GPIO_Handle);

    GPIO_Handle.GPIO_Config.PinNumber = 3;
    GPIO_Init(&GPIO_Handle);

    GPIO_Handle.GPIO_Config.PinNumber = 4;
    GPIO_Handle.GPIO_Config.PinMode = GPIO_MODER_OUTPUT;
    GPIO_Init(&GPIO_Handle);
}

void USART_Config(){
    USART_Handle.config.wordlen = 8;
    USART_Handle.config.baudrate = 115200;
    USART_Handle.config.stopBits = 1;
    USART_Handle.config.parityEnable = 0;
    USART_Handle.config.interrupt = 1;
    USART_Handle.pUSART = USART2;

    USART_Init(&USART_Handle);
}

int main (){
    GPIO_Config();
    USART_Config();

    for (;;) {
        delay_ms(10);
    }
    return 0;
}
