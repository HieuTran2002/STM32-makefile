#ifndef STM32G431XX_H
#define STM32G431XX_H

#include "stm32g431xx_types.h"

#define AHB1_BASEADDR       0x40020000U
#define RCC_BASEADDR        0X40021000U
#define RCC                 ((RCC_TypeDef *) RCC_BASEADDR)

#define AHB2_BASEADDR       0x48000000U
#define GPIOA_BASEADDR      AHB2_BASEADDR
#define GPIOC_BASEADDR      (AHB2_BASEADDR + 0x0800U)
#define GPIOA               ((GPIO_Type *) GPIOA_BASEADDR)
#define GPIOC               ((GPIO_Type *) GPIOC_BASEADDR)

#define SYSTICK_BASEADDR    0xE000E010UL
#define SysTick             ((SysTick_Type *) SYSTICK_BASEADDR)


#endif /* end of include guard: STM32G431XX_H */
