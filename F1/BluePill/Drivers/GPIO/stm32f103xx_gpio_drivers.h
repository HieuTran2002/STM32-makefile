#ifndef STM32F103XX_GPIO_DRIVERS_H
#define STM32F103XX_GPIO_DRIVERS_H

#include "stm32f103xb.h"
#include <stdint.h>

#define GPIOA_PCLK_EN()             RCC->APB2ENR |= 1 << 2;
#define GPIOB_PCLK_EN()             RCC->APB2ENR |= 1 << 3;
#define GPIOC_PCLK_EN()             RCC->APB2ENR |= 1 << 4;
#define GPIOD_PCLK_EN()             RCC->APB2ENR |= 1 << 5;

#define GPIO_MODE_INPUT             0
#define GPIO_MODE_OUTPUT_10M        1
#define GPIO_MODE_OUTPUT_2M         2
#define GPIO_MODE_OUTPUT_50M        3

#define GPIO_CNF_OUTPUT_PP              0
#define GPIO_CNF_OUTPUT_OD              1
#define GPIO_CNF_ALTFNC_PP              2
#define GPIO_CNF_ALTFNC_OD              3

typedef struct {
    uint8_t PinNumber;
    uint8_t PinMode;
    uint8_t CNF;
} GPIO_Config_Type;

typedef struct{
    GPIO_Config_Type pGPIO_Config;
    GPIO_TypeDef* pGPIO;
} GPIO_Handle_Type;

void GPIO_init(GPIO_Handle_Type* pGPIO_Handle);

void GPIO_Clock_Control(GPIO_TypeDef* pGPIO, uint8_t EnOrDi);

#endif /* end of include guard: STM32F103XX_GPIO_DRIVERS_H */
