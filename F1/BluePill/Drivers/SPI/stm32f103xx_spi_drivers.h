#ifndef STM32F411XX_SPI_DRIVER_H
#define STM32F411XX_SPI_DRIVER_H

#include "stm32f1xx.h"
#include <stdint.h>


#define SPI_DEVICE_MODE_SLAVE                   0
#define SPI_DEVICE_MODE_MASTER                  1

#define SPI_BUS_CONFIG_FD                       1
#define SPI_BUS_CONFIG_HD                       2
#define SPI_BUS_CONFIG_SIMPLES_RXONLY           4

#define SPI_SCLK_SPEED_DIV2                     0
#define SPI_SCLK_SPEED_DIV4                     1
#define SPI_SCLK_SPEED_DIV8                     2
#define SPI_SCLK_SPEED_DIV16                    3
#define SPI_SCLK_SPEED_DIV32                    4
#define SPI_SCLK_SPEED_DIV64                    5
#define SPI_SCLK_SPEED_DIV128                   6
#define SPI_SCLK_SPEED_DIV256                   7

#define SPI_DFF_8BITS                           0
#define SPI_DFF_16BITS                          1

#define SPI_CPOL_LOW                            0
#define SPI_CPOL_HIGH                           1 

#define SPI_CPHA_LOW                            0
#define SPI_CPHA_HIGH                           1 


/* SPI1 */
#define SPI1_PORT                               GPIOA
#define SPI1_MOSI                               5
#define SPI1_MISO                               6
#define SPI1_SCK                                7

typedef struct{
    uint8_t DeviceMode;
    uint8_t BusConfig;
    uint8_t SclkSpeed;
    uint8_t DFF;
    uint8_t CPOL;
    uint8_t CPHA;
    uint8_t SSM;
} SPI_Config_Type;

typedef struct{
    SPI_TypeDef* pSPI;
    SPI_Config_Type SPI_Config;
} SPI_Handle_Type;

void SPI_Init(SPI_Handle_Type* pSPI_Handle);

void SPI_SendData(SPI_TypeDef* pSPI, uint8_t* pTxBuffer, uint8_t Len);

void SPI_RecieveData(SPI_TypeDef* pSPI, uint8_t* pTxBuffer, uint8_t Len);

#endif

