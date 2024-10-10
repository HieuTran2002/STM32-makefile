#ifndef STM32F411XX_H
#define STM32F411XX_H

#include "stm32f411xx_types.h"

/* -------------------- AHB1 -------------------- */
#define AHB1_BASEADDR                       0x40020000U
#define FLASH_BASEADDR                      (AHB1_BASEADDR + 0x3C00U) 
#define RCC_BASEADDR                        (AHB1_BASEADDR + 0x3800UL)


#define GPIOA_BASEADDR                      AHB1_BASEADDR 
#define GPIOB_BASEADDR                      (AHB1_BASEADDR + 0x0400U) 
#define GPIOC_BASEADDR                      (AHB1_BASEADDR + 0x0800U) 
#define GPIOD_BASEADDR                      (AHB1_BASEADDR + 0x0C00U) 


/* -------------------- APB1 -------------------- */
#define APB1_BASEADDR                       0x40000000U

#define SPI2_BASEADDR                       (APB1_BASEADDR + 0x3800U)
#define SPI3_BASEADDR                       (APB1_BASEADDR + 0x3C00U)

#define I2C3_BASEADDR                       (APB1_BASEADDR + 0x5C00U)
#define I2C2_BASEADDR                       (APB1_BASEADDR + 0x5800U)
#define I2C1_BASEADDR                       (APB1_BASEADDR + 0x5400U)


/* -------------------- APB2 -------------------- */
#define APB2_BASEADDR                       0x40010000U

#define SPI1_BASEADDR                       (APB2_BASEADDR + 0x3000U)
#define SPI4_BASEADDR                       (APB2_BASEADDR + 0x3400U)


/* -------------------- Core -------------------- */
#define SYSTICK_BASEADDR                    0xE000E010UL


/* ------------ Peripheral pointers ------------- */
#define SysTick                             ((SysTick_Type *) SYSTICK_BASEADDR)
#define RCC                                 ((RCC_Type *) RCC_BASEADDR)
#define FLASH                               ((FLASH_Type *) FLASH_BASEADDR)


/* ------------ GPIO ------------- */
#define GPIOA                               ((GPIO_Type *) GPIOA_BASEADDR) 
#define GPIOB                               ((GPIO_Type *) GPIOB_BASEADDR) 
#define GPIOC                               ((GPIO_Type *) GPIOC_BASEADDR) 
#define GPIOD                               ((GPIO_Type *) GPIOD_BASEADDR) 

#define GPIOA_PCLK_EN()                     RCC->AHB1ENR |= 1
#define GPIOB_PCLK_EN()                     RCC->AHB1ENR |= 1 << 1
#define GPIOC_PCLK_EN()                     RCC->AHB1ENR |= 1 << 2
#define GPIOD_PCLK_EN()                     RCC->AHB1ENR |= 1 << 3

/* ------------ SPI ------------- */
#define SPI1                                ((SPI_Type *) SPI1_BASEADDR)
#define SPI2                                ((SPI_Type *) SPI2_BASEADDR)
#define SPI3                                ((SPI_Type *) SPI3_BASEADDR)
#define SPI4                                ((SPI_Type *) SPI4_BASEADDR)

/* ------------ I2C ------------- */
#define I2C1                                ((I2C_Type*) I2C1_BASEADDR)
#define I2C2                                ((I2C_Type*) I2C2_BASEADDR)
#define I2C3                                ((I2C_Type*) I2C3_BASEADDR)

#define SPI1_PCLK_EN()                      RCC->APB2ENR |= 1 << 12
#define SPI2_PCLK_EN()                      RCC->APB1ENR |= 1 << 14
#define SPI3_PCLK_EN()                      RCC->APB1ENR |= 1 << 15
#define SPI4_PCLK_EN()                      RCC->APB2ENR |= 1 << 13


#endif
