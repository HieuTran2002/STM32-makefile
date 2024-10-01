#include "stdint.h"
#include "stm32g431xx.h"
#include "stm32g431_gpio_drivers.h"
#include "stm32g431xx_clock.h"
#include "bsp.h"

#define BTN_PRESSED !((GPIOC->IDR >> 13) & 1)

void GPIO_Config(void){
    GPIO_Handle_Type GPIO_Handle;

    GPIO_Handle.pGPIO                   = LED_PORT;
    GPIO_Handle.GPIO_Config.PinNumber   = LED_PIN;
    GPIO_Handle.GPIO_Config.PinMode     = GPIO_MODER_OUTPUT;
    GPIO_Handle.GPIO_Config.Speed       = GPIO_OSPEED_FAST;
    GPIO_Handle.GPIO_Config.Pull        = GPIO_PUPDR_NONE;
    GPIO_Handle.GPIO_Config.OType       = GPIO_OTYPE_PP;

    GPIO_Init(&GPIO_Handle);
}

int main(void){
    GPIO_Config();
    while (1) {
        LED_PORT->ODR ^=  1 << LED_PIN;
        delay_ms(100);
    }
    return 1;
}
