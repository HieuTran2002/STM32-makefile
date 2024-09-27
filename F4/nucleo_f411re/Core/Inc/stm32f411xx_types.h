#ifndef STM32F411XX_TYPES_H
#define STM32F411XX_TYPES_H

#include <stdint.h>

#define __vo volatile


typedef struct {
  __vo uint32_t CR;            
  __vo uint32_t PLLCFGR;       
  __vo uint32_t CFGR;          
  __vo uint32_t CIR;           
  __vo uint32_t AHB1RSTR;      
  __vo uint32_t AHB2RSTR;      
  uint32_t      RESERVED0[2];     
  __vo uint32_t APB1RSTR;      
  __vo uint32_t APB2RSTR;      
  uint32_t      RESERVED1[2];  
  __vo uint32_t AHB1ENR;       
  __vo uint32_t AHB2ENR;       
  uint32_t      RESERVED2[2];     
  __vo uint32_t APB1ENR;       
  __vo uint32_t APB2ENR;       
  uint32_t      RESERVED3[2];  
  __vo uint32_t AHB1LPENR;     
  __vo uint32_t AHB2LPENR;     
  uint32_t      RESERVED4[2];     
  __vo uint32_t APB1LPENR;     
  __vo uint32_t APB2LPENR;     
  uint32_t      RESERVED5[2];  
  __vo uint32_t BDCR;          
  __vo uint32_t CSR;           
  uint32_t      RESERVED6[2];  
  __vo uint32_t SSCGR;         
  __vo uint32_t PLLI2SCFGR;    

} RCC_Type;


typedef struct {
  __vo uint32_t ACR;      
  __vo uint32_t KEYR;     
  __vo uint32_t OPTKEYR;  
  __vo uint32_t SR;       
  __vo uint32_t CR;       
  __vo uint32_t OPTCR;    
  __vo uint32_t OPTCR1;   
} FLASH_Type;


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


typedef struct {
  __vo uint32_t CR1;        
  __vo uint32_t CR2;        
  __vo uint32_t SR;         
  __vo uint32_t DR;         
  __vo uint32_t CRCPR;      
  __vo uint32_t RXCRCR;     
  __vo uint32_t TXCRCR;     
  __vo uint32_t I2SCFGR;    
  __vo uint32_t I2SPR;      
} SPI_Type;

typedef struct
{
  __vo uint32_t CTRL;
  __vo uint32_t LOAD;
  __vo uint32_t VAL;
  __vo uint32_t CALIB;
} SysTick_Type;

#endif
