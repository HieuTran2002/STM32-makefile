#include "stm32g431_gpio_drivers.h"
#include "stm32g431xx.h"
#include <stdint.h>

void GPIO_Init(GPIO_Handle_Type* pGPIO_Handle){
    uint32_t temp = 0;

    GPIO_Clock_Control(pGPIO_Handle->pGPIO, 1);

    if(pGPIO_Handle->GPIO_Config.PinMode <= GPIO_MODER_ANALOG){
        /* Interrupt mode */

        temp |= pGPIO_Handle->GPIO_Config.PinMode << (2 * pGPIO_Handle->GPIO_Config.PinNumber);
        pGPIO_Handle->pGPIO->MODER &= ~(0b11 << (2 * pGPIO_Handle->GPIO_Config.PinNumber));
        pGPIO_Handle->pGPIO->MODER |= temp;

        temp = 0;
        temp |= pGPIO_Handle->GPIO_Config.Pull << (2 * pGPIO_Handle->GPIO_Config.PinNumber);
        pGPIO_Handle->pGPIO->PUPDR &= ~(0x3 << (2 * pGPIO_Handle->GPIO_Config.PinNumber));
        pGPIO_Handle->pGPIO->PUPDR |= temp;

        temp = 0;
        temp |= pGPIO_Handle->GPIO_Config.OType << pGPIO_Handle->GPIO_Config.PinNumber;
        pGPIO_Handle->pGPIO->OTYPER &= ~(1 << pGPIO_Handle->GPIO_Config.PinNumber);
        pGPIO_Handle->pGPIO->OTYPER |= temp;

        temp = 0;
        temp |= pGPIO_Handle->GPIO_Config.Speed << (2 * pGPIO_Handle->GPIO_Config.PinNumber);
        pGPIO_Handle->pGPIO->OSPEEDR &= ~(0x3 << (2 * pGPIO_Handle->GPIO_Config.PinNumber));
        pGPIO_Handle->pGPIO->OSPEEDR |= temp;

        temp = 0;
        if (pGPIO_Handle->GPIO_Config.PinMode == GPIO_MODER_ALTFNC) {
            uint8_t idx1, idx2;
            idx1 = pGPIO_Handle->GPIO_Config.PinNumber / 8;
            idx2 = pGPIO_Handle->GPIO_Config.PinNumber % 8;

            pGPIO_Handle->pGPIO->AFR[idx1] &= ~(0xF << (idx2 * 4));
            pGPIO_Handle->pGPIO->AFR[idx1] |= (pGPIO_Handle->GPIO_Config.AltFunc << (idx2 * 4));
        }
    }
    else{
        /* Interrupt mode */

    }
}


void GPIO_Clock_Control(GPIO_TypeDef* pGPIO, uint8_t EnOrDi){
    if (EnOrDi) {
        if      (pGPIO == GPIOA) { GPIOA_PCLK_EN(); }
        else if (pGPIO == GPIOB) { GPIOB_PCLK_EN(); }
        else if (pGPIO == GPIOC) { GPIOC_PCLK_EN(); }
        else if (pGPIO == GPIOD) { GPIOD_PCLK_EN(); }
    }
}
