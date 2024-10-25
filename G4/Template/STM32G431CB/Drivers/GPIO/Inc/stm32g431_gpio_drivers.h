#ifndef STM32G41_GPIO_DRIVERS_H
#define STM32G41_GPIO_DRIVERS_H

#include "stm32g431xx.h"

#define GPIOA_PCLK_EN()          RCC->AHB2ENR |= 1 << 0
#define GPIOB_PCLK_EN()          RCC->AHB2ENR |= 1 << 1
#define GPIOC_PCLK_EN()          RCC->AHB2ENR |= 1 << 2
#define GPIOD_PCLK_EN()          RCC->AHB2ENR |= 1 << 3


#endif /* end of include guard: STM32G41_GPIO_DRIVERS_H */
