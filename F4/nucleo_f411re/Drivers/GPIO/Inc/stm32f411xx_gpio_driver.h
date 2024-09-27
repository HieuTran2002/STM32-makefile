#ifndef STM32F411XX_GPIO_DRIVER_H
#define STM32F411XX_GPIO_DRIVER_H

#include "stm32f411xx.h"
#include "stm32f411xx_types.h"
#include <stdint.h>


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
    GPIO_Type* pGPIO;
    GPIO_Config_Type GPIO_Config;
} GPIO_Handle_Type;


// Read and write
void GPIO_TogglePin(GPIO_Type *pGPIO, uint8_t PinNumber);
void GPIO_WritePin(GPIO_Type *pGPIO, uint8_t PinNumber, uint8_t State);
uint8_t GPIO_ReadPin(GPIO_Type *pGPIO, uint8_t PinNumber);

void GPIO_Init(GPIO_Handle_Type* pGPIO_Handle);
void GPIO_Deinit(GPIO_Config_Type* pGPIO_Config);

#endif
