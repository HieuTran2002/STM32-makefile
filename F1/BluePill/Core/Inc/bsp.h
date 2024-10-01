#ifndef BSP_H
#define BSP_H

#include "stm32f103xb.h"
#include "stm32f103xx_gpio_drivers.h"
#include "stm32f103xx_spi_drivers.h"

#define KEY_PIN 0
#define KEY_PORT GPIOA
#define KEY_PRESSED     !(GPIOA->IDR & 0b1)

#define LED_PIN 2
#define LED_PORT GPIOB

void SPI1_PIN_Init(void){
    /* Enable GPIOA */
    GPIOA_PCLK_EN();

    /* 
     * PA4 NSS  
     * PA5 SCK  
     * PA6 MISO
     * PA7 MOSI
     */

    /* High speed output, alternative mode, push-pull */
    GPIOA->CRL |= (0b1111 << (4 * 4));
    GPIOA->CRL |= (0b1111 << (5 * 4));
    GPIOA->CRL |= (0b1111 << (6 * 4));
    GPIOA->CRL |= (0b1111 << (7 * 4));
}

void SPI1_Init(void){
    SPI_Handle_Type SPI_Handle;

    SPI_Handle.pSPI =  SPI1;

    SPI_Handle.SPI_Config.DeviceMode = SPI_DEVICE_MODE_SLAVE;
    SPI_Handle.SPI_Config.BusConfig = SPI_BUS_CONFIG_FD;
    SPI_Handle.SPI_Config.DFF = SPI_DFF_8BITS;
    SPI_Handle.SPI_Config.SSM = 0;
    SPI_Handle.SPI_Config.CPOL = 0;
    SPI_Handle.SPI_Config.CPHA = 0;

    SPI_Init(&SPI_Handle);

    SPI1->CR1 |= 1 << 6;
}

void LED_Init(){
    GPIO_Handle_Type GPIO_Handle;

    GPIO_Handle.pGPIO = LED_PORT;
    GPIO_Handle.pGPIO_Config.PinNumber  = LED_PIN;
    GPIO_Handle.pGPIO_Config.PinMode    = GPIO_MODE_OUTPUT_2M;
    GPIO_Handle.pGPIO_Config.CNF        = GPIO_CNF_OUTPUT_PP;
    GPIO_init(&GPIO_Handle);
}

#endif /* end of include guard: BSP_H */
