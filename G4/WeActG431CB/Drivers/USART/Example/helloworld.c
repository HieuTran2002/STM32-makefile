#include "stm32g431_gpio_drivers.h"
#include "bsp.h"
#include "stm32g431xx.h"
#include "stm32g431xx_clock.h"
#include "stm32g4xx_driver_usart.h"
#include <stdint.h>
#include "string.h"

void GPIO_Config(){
    GPIO_Handle_Type gpio;
    gpio.pGPIO = LED_PORT;
    gpio.GPIO_Config.PinNumber = LED_PIN;
    gpio.GPIO_Config.PinMode = 1;
    gpio.GPIO_Config.OType = 0;

    GPIO_Init(&gpio);
}

void USART_Pin_Config(){
    GPIO_Handle_Type usart_pin;

    /* PA2 = TX, PA3 = RX */

    usart_pin.pGPIO =  GPIOA;
    usart_pin.GPIO_Config.PinNumber = 2;
    usart_pin.GPIO_Config.PinMode = 2;
    usart_pin.GPIO_Config.Pull = 1;
    usart_pin.GPIO_Config.OType = 0;
    usart_pin.GPIO_Config.Speed = 3;
    usart_pin.GPIO_Config.AltFunc = 7;
    GPIO_Init(&usart_pin);

    usart_pin.GPIO_Config.PinNumber = 3;
    GPIO_Init(&usart_pin);

    GPIOA->AFR[0] |= (7 << 8) | (7 << 12);
}

USART_Handle_Type usart;
void USART_Config(){
    USART_Pin_Config();

    usart.pUSART = USART2;
    usart.USART_Config.Baudrate = USART_BAUDRATE_9600;
    usart.USART_Config.Mode = USART_RXTX;
    usart.USART_Config.StopBits = USART_STOPBITS_1;
    usart.USART_Config.Wordlen = USART_WORDLEN_8;
    usart.USART_Config.Parity = USART_Parity_None;

    USART_Init(&usart);
}
int main(){
    GPIO_Config();
    USART_Config();

    char* str = "hello";
    // uint8_t a = 0x81;

    for (;;) {
        USART_SendData(&usart, (uint8_t*)str, strlen(str));
        LED_OFF();
        delay_ms(500);
    }
    return 0;
}
