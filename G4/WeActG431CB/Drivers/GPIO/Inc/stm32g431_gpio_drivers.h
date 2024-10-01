#ifndef STM32G41_GPIO_DRIVERS_H
#define STM32G41_GPIO_DRIVERS_H

#include "stm32g431xx.h"

/* GPIO Clock control */
#define GPIOA_PCLK_EN()          RCC->AHB2ENR |= 1 << 0
#define GPIOB_PCLK_EN()          RCC->AHB2ENR |= 1 << 1
#define GPIOC_PCLK_EN()          RCC->AHB2ENR |= 1 << 2
#define GPIOD_PCLK_EN()          RCC->AHB2ENR |= 1 << 3

/* Configuration options */
#define GPIO_MODER_INPUT                0
#define GPIO_MODER_OUTPUT               1
#define GPIO_MODER_ALTFNC               2
#define GPIO_MODER_ANALOG               3
#define GPIO_MODER_IT_FT                4
#define GPIO_MODER_IT_RT                5
#define GPIO_MODER_IT_RFT               6

#define GPIO_OTYPE_PP                   0
#define GPIO_OTYPE_OD                   1

#define GPIO_OSPEED_LOW                 0
#define GPIO_OSPEED_MEDIUM              1
#define GPIO_OSPEED_FAST                2
#define GPIO_OSPEED_HIGH                3

#define GPIO_PUPDR_NONE                 0
#define GPIO_PUPDR_PU                   1
#define GPIO_PUPDR_PD                   2


typedef struct{
    uint32_t PinNumber;
    uint32_t PinMode;
    uint32_t Pull;
    uint32_t Speed;
    uint32_t OType;
    uint32_t AltFunc;
} GPIO_Config_Type;

typedef struct{
    GPIO_TypeDef* pGPIO;
    GPIO_Config_Type GPIO_Config;
} GPIO_Handle_Type;

void GPIO_Init(GPIO_Handle_Type* pGPIO_Handle);
void GPIO_Clock_Control(GPIO_TypeDef* pGPIO, uint8_t EnOrDi);
#endif /* end of include guard: STM32G41_GPIO_DRIVERS_H */
