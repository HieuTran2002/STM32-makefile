#include "stm32f103xb.h"
#include "stm32f103xx_spi_drivers.h"
#include "stm32f1xx_clock.h"
#include <stdint.h>

void Clock_Config(void){
    /* Reset CR */
    RCC->CR &= ~1U;

    /* Enable HSI */
    RCC->CR |= 1;

    /* Wait for HSI */
    while (!(RCC->CR & (1 << 1)));
}

void GPIO_Config(void){
    /* Enable GPIOA->D */
    RCC->APB2ENR |= 0xF << 2;

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

    /* Config PB2 as Output */
    GPIOB->CRL      |= (2 << 8);
    GPIOB->CRL      &= ~(1 << 10);
}

void GPIO_Config2(void){
    /* Enable GPIOA->D */
    RCC->APB2ENR |= 0xF << 2;
    /* 
     * PA4 (NSS) -> Input Floating
     * PA5 (SCK) -> Input Floating
     * PA6 (MISO) -> Alternate function push-pull
     * PA7 (MOSI) -> Input Floating
     */

    // Configure PA4 as Input Floating (NSS)
    GPIOA->CRL &= ~(0xF << (4 * 4));
    GPIOA->CRL |= (0b0100 << (4 * 4));

    // Configure PA5 as Input Floating (SCK)
    GPIOA->CRL &= ~(0xF << (5 * 4));
    GPIOA->CRL |= (0b0100 << (5 * 4));

    // Configure PA6 as Alternate Function Push-Pull (MISO)
    GPIOA->CRL &= ~(0xF << (6 * 4));
    GPIOA->CRL |= (0b1001 << (6 * 4));

    // Configure PA7 as Input Floating (MOSI)
    GPIOA->CRL &= ~(0xF << (7 * 4));
    GPIOA->CRL |= (0b0100 << (7 * 4));

    // Config PB2 as Output for toggling the LED
    GPIOB->CRL &= ~(0xF << (2 * 4)); // Clear configuration
    GPIOB->CRL |= (0b0010 << (2 * 4)); // General-purpose output push-pull
}


void SPI_Config(void){
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

void SPI_SlaveReceive(uint8_t* RecievedData) {
    // Wait until RXNE (Receive buffer not empty)
    while (!(SPI1->SR & 1));

    // Read data from the data register
    *RecievedData = (uint8_t)SPI1->DR;
}

void LED(uint8_t state){
    if (state) {
        GPIOB->ODR ^= (1 << 2);
    }
}

int main(void)
{
    Clock_Config();
    // GPIO_Config();
    GPIO_Config();
    SPI_Config();


    char value = 0;
    uint8_t value2 = 122;
    uint8_t sendData;
    while (1) {
        // SPI_SlaveReceive(&value);
        SPI_RecieveData(SPI1, &value, 5);
        sendData = value2 + 1;
        LED(value);
        SPI_SendData(SPI1, &sendData, 1);
    }
    return 0;
}
