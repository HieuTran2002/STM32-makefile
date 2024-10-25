#ifndef STM32G431XX_TYPES_H
#define STM32G431XX_TYPES_H

#include <stdint.h>

#define __vo                volatile

/* typedef */
typedef struct
{
  __vo uint32_t CTRL;
  __vo uint32_t LOAD;
  __vo uint32_t VAL;
  __vo uint32_t CALIB;
} SysTick_Type;

typedef struct
{
  __vo uint32_t CR;          /*!< RCC clock control register,                                              Address offset: 0x00 */
  __vo uint32_t ICSCR;       /*!< RCC internal clock sources calibration register,                         Address offset: 0x04 */
  __vo uint32_t CFGR;        /*!< RCC clock configuration register,                                        Address offset: 0x08 */
  __vo uint32_t PLLCFGR;     /*!< RCC system PLL configuration register,                                   Address offset: 0x0C */
  uint32_t      RESERVED0;   /*!< Reserved,                                                                Address offset: 0x10 */
  uint32_t      RESERVED1;   /*!< Reserved,                                                                Address offset: 0x14 */
  __vo uint32_t CIER;        /*!< RCC clock interrupt enable register,                                     Address offset: 0x18 */
  __vo uint32_t CIFR;        /*!< RCC clock interrupt flag register,                                       Address offset: 0x1C */
  __vo uint32_t CICR;        /*!< RCC clock interrupt clear register,                                      Address offset: 0x20 */
  uint32_t      RESERVED2;   /*!< Reserved,                                                                Address offset: 0x24 */
  __vo uint32_t AHB1RSTR;    /*!< RCC AHB1 peripheral reset register,                                      Address offset: 0x28 */
  __vo uint32_t AHB2RSTR;    /*!< RCC AHB2 peripheral reset register,                                      Address offset: 0x2C */
  __vo uint32_t AHB3RSTR;    /*!< RCC AHB3 peripheral reset register,                                      Address offset: 0x30 */
  uint32_t      RESERVED3;   /*!< Reserved,                                                                Address offset: 0x34 */
  __vo uint32_t APB1RSTR1;   /*!< RCC APB1 peripheral reset register 1,                                    Address offset: 0x38 */
  __vo uint32_t APB1RSTR2;   /*!< RCC APB1 peripheral reset register 2,                                    Address offset: 0x3C */
  __vo uint32_t APB2RSTR;    /*!< RCC APB2 peripheral reset register,                                      Address offset: 0x40 */
  uint32_t      RESERVED4;   /*!< Reserved,                                                                Address offset: 0x44 */
  __vo uint32_t AHB1ENR;     /*!< RCC AHB1 peripheral clocks enable register,                              Address offset: 0x48 */
  __vo uint32_t AHB2ENR;     /*!< RCC AHB2 peripheral clocks enable register,                              Address offset: 0x4C */
  __vo uint32_t AHB3ENR;     /*!< RCC AHB3 peripheral clocks enable register,                              Address offset: 0x50 */
  uint32_t      RESERVED5;   /*!< Reserved,                                                                Address offset: 0x54 */
  __vo uint32_t APB1ENR1;    /*!< RCC APB1 peripheral clocks enable register 1,                            Address offset: 0x58 */
  __vo uint32_t APB1ENR2;    /*!< RCC APB1 peripheral clocks enable register 2,                            Address offset: 0x5C */
  __vo uint32_t APB2ENR;     /*!< RCC APB2 peripheral clocks enable register,                              Address offset: 0x60 */
  uint32_t      RESERVED6;   /*!< Reserved,                                                                Address offset: 0x64 */
  __vo uint32_t AHB1SMENR;   /*!< RCC AHB1 peripheral clocks enable in sleep and stop modes register,      Address offset: 0x68 */
  __vo uint32_t AHB2SMENR;   /*!< RCC AHB2 peripheral clocks enable in sleep and stop modes register,      Address offset: 0x6C */
  __vo uint32_t AHB3SMENR;   /*!< RCC AHB3 peripheral clocks enable in sleep and stop modes register,      Address offset: 0x70 */
  uint32_t      RESERVED7;   /*!< Reserved,                                                                Address offset: 0x74 */
  __vo uint32_t APB1SMENR1;  /*!< RCC APB1 peripheral clocks enable in sleep mode and stop modes register 1, Address offset: 0x78 */
  __vo uint32_t APB1SMENR2;  /*!< RCC APB1 peripheral clocks enable in sleep mode and stop modes register 2, Address offset: 0x7C */
  __vo uint32_t APB2SMENR;   /*!< RCC APB2 peripheral clocks enable in sleep mode and stop modes register, Address offset: 0x80 */
  uint32_t      RESERVED8;   /*!< Reserved,                                                                Address offset: 0x84 */
  __vo uint32_t CCIPR;       /*!< RCC peripherals independent clock configuration register,                Address offset: 0x88 */
  uint32_t      RESERVED9;   /*!< Reserved,                                                                Address offset: 0x8C */
  __vo uint32_t BDCR;        /*!< RCC backup domain control register,                                      Address offset: 0x90 */
  __vo uint32_t CSR;         /*!< RCC clock control & status register,                                     Address offset: 0x94 */
  __vo uint32_t CRRCR;       /*!< RCC clock recovery RC register,                                          Address offset: 0x98 */
  __vo uint32_t CCIPR2;      /*!< RCC peripherals independent clock configuration register 2,              Address offset: 0x9C */
} RCC_TypeDef;


typedef struct {
  __vo uint32_t MODER;    
  __vo uint32_t OTYPER;   
  __vo uint32_t OSPEEDR;  
  __vo uint32_t PUPDR;    
  __vo uint32_t IDR;      
  __vo uint32_t ODR;      
  __vo uint32_t BSRR;     
  __vo uint32_t LCKR;     
  __vo uint32_t AFR[2];   
} GPIO_Type;

#endif /* end of include guard: STM32G431XX_TYPES_H */
