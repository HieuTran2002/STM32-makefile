#include "stm32g030_gpio_driver.h"
#include "stm32g030_spi_driver.h"
#include "stm32g030xx.h"
#include "stm32g030_clock.h"
#include <stdint.h>


void Clock_Config(void){
    RCC->CR |= 1;
    while(!(RCC->CR & (1 << 1)));

    RCC->CFGR &= ~(1);
}

void GPIO_Config(void){
    RCC->IOPENR |= 0b1;

    GPIO_Handle_Type GPIO_Handle;

    GPIO_Handle.pGPIO                           = GPIOA;
    GPIO_Handle.GPIO_Config.PinNumber           = 4;
    GPIO_Handle.GPIO_Config.PinMode             = GPIO_MODER_OUTPUT;
    GPIO_Init(&GPIO_Handle);
}

void SPI_Pin_Config(void){
    /* PA3 SPI2_MISO
     * PB7 SPI2_MOSI
     * PA0 SPI2_SCK
     * */

    /* Enable GPIOA+B */
    RCC->IOPENR |= 3;

    GPIO_Handle_Type SPI_GPIO_Handle;
    SPI_GPIO_Handle.pGPIO = GPIOA;

    SPI_GPIO_Handle.GPIO_Config.PinMode         = GPIO_MODER_ALTFNC;
    SPI_GPIO_Handle.GPIO_Config.Pull            = GPIO_PUPDR_NONE;
    SPI_GPIO_Handle.GPIO_Config.OType           = GPIO_OTYPE_PP;
    SPI_GPIO_Handle.GPIO_Config.Speed           = GPIO_OSPEED_MEDIUM;


    SPI_GPIO_Handle.GPIO_Config.PinNumber       = 0;
    SPI_GPIO_Handle.GPIO_Config.AltFunc         = 0;
    GPIO_Init(&SPI_GPIO_Handle);

    SPI_GPIO_Handle.GPIO_Config.PinNumber       = 3;
    SPI_GPIO_Handle.GPIO_Config.AltFunc         = 0;
    GPIO_Init(&SPI_GPIO_Handle);

    SPI_GPIO_Handle.pGPIO                       = GPIOB;
    SPI_GPIO_Handle.GPIO_Config.PinNumber       = 7;
    SPI_GPIO_Handle.GPIO_Config.AltFunc         = 1;
    GPIO_Init(&SPI_GPIO_Handle);
}

void SPI_Config(void){
    RCC->APBENR1 |= (1 << 14);

    SPI_Handle_Type SPI_Handle;

    SPI_Handle.pSPI                             = SPI2;
    SPI_Handle.SPI_Config.DeviceMode            = SPI_DEVICE_MODE_MASTER;
    SPI_Handle.SPI_Config.CPOL                  = 0;
    SPI_Handle.SPI_Config.CPHA                  = 0;
    SPI_Handle.SPI_Config.BusConfig             = SPI_BUS_CONFIG_FD;
    SPI_Handle.SPI_Config.SclkSpeed             = 7; 
    SPI_Handle.SPI_Config.DFF                   = SPI_DFF_8BITS;
    SPI_Handle.SPI_Config.SSM                   = 1;

    SPI_Init(&SPI_Handle);
}

void SPI2_Init(void){
    uint32_t temp = 0;

    /* Enable SPI2 Clock */
    RCC->APBENR1 |= (1 << 14);  // Enable SPI2 clock

    /* Configure SPI2 as Master, 8-bit data, SSM enabled, CPOL/CPHA low */
    temp = 0;  // Clear control register

    temp |= (1 << 2);  /* Device mode | master or BET */ 

    temp |= (7 << 3); /* Baudrate */

    temp &= ~(1 << 11); /* CRCL = 8 bits */ 

    temp |= (1 << 9);  /* SSM = 1 (Software Slave Management) */ 

    temp |= (1 << 8);  /* SSI = 1 (Internal Slave Select) */ 

    temp |= (1 << 6);  // SPE = 1 (Enable SPI)

    SPI2->CR1 |= temp;
    SPI2->CR2 |= 0b0111 << 8;
}


int main(void)
{
    // Clock_Config();
    GPIO_Config();
    SPI_Pin_Config();
    SPI2_Init();

    char string[] = "hello world\n";

    while (1) {
        SPI_SendData(SPI2, (uint8_t *)string, sizeof(string) - 1);
        GPIO_TogglePin(GPIOA, 4);
        delay_ms(100);
    }
}
