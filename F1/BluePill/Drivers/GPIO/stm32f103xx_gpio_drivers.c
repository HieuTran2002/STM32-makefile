#include "stm32f103xx_gpio_drivers.h"
#include "stm32f103xb.h"

void GPIO_init(GPIO_Handle_Type* pGPIO_Handle){
    uint32_t temp = 0;

    /* Enable the corresponding port */
    GPIO_Clock_Control(pGPIO_Handle->pGPIO, 1);

    temp = (pGPIO_Handle->pGPIO_Config.CNF << 2 | pGPIO_Handle->pGPIO_Config.PinMode) << (pGPIO_Handle->pGPIO_Config.PinMode * 4);

    pGPIO_Handle->pGPIO->CRL &= ~(0xF << (pGPIO_Handle->pGPIO_Config.PinNumber * 4));
    pGPIO_Handle->pGPIO->CRL |= temp;
}

void GPIO_Clock_Control(GPIO_TypeDef* pGPIO, uint8_t EnOrDi){
    if      (pGPIO == GPIOA)    { GPIOA_PCLK_EN(); }
    else if (pGPIO == GPIOB)    { GPIOB_PCLK_EN(); }
    else if (pGPIO == GPIOC)    { GPIOC_PCLK_EN(); }
    else if (pGPIO == GPIOC)    { GPIOD_PCLK_EN(); }
}
