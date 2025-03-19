#include "stdint.h"
#include "stm32g031_gpio_driver.h"
#include "stm32g031_clock.h"

void Init_GPIO(){
    GPIO_Handle_Type config;

    config.pGPIO = GPIOA;
    config.GPIO_Config.OType = GPIO_OTYPE_PP;
    config.GPIO_Config.PinMode = GPIO_MODER_OUTPUT;
    config.GPIO_Config.PinNumber = 4;
    GPIO_Init(&config);
}

int main(){
    Init_GPIO();
    for (;;) {
        GPIOA->ODR ^= (1 << 4);
        delay_ms(50);
    }
}
