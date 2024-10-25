#include "stm32f411xx.h"
#include "stm32f411xx_clock.h"
#include "stm32f411xx_gpio_driver.h"
#include "stm32f411xx_i2c_driver.h"
#include <stdint.h>
#include "bsp.h"
#include "string.h"
#include "core.h"

#define BTN2     ((GPIOB->IDR >> 8) & 1)

char greeting[] = "Hello world\n";
char goodbye[]  = "Goodbye world\n";

uint8_t call_count = 0;
uint8_t ev_count = 0;

I2C_Handle_Type I2CHandle = {0};

/* My own way of doing polling. */
typedef struct {
    uint8_t* pRxBuffer;
    uint8_t* pTxBuffer;
} I2C_Data;

I2C_Data I2CData;
uint8_t a = 0xF;
uint8_t* rxBuffer = &a;

/* 
 * PB6 SCL | PB7 SDA
 */
void I2C_Pin_Config(void){
    GPIO_Handle_Type I2C_PinConfig;

    I2C_PinConfig.pGPIO                 = GPIOB;
    I2C_PinConfig.GPIO_Config.Pull      = GPIO_PUPDR_PU;
    I2C_PinConfig.GPIO_Config.OType     = GPIO_OTYPE_OD;
    I2C_PinConfig.GPIO_Config.PinMode   = GPIO_MODER_ALTFNC;
    I2C_PinConfig.GPIO_Config.AltFunc   = 4;
    
    I2C_PinConfig.GPIO_Config.PinNumber = 6;
    GPIO_Init(&I2C_PinConfig);

    I2C_PinConfig.GPIO_Config.PinNumber = 7;
    GPIO_Init(&I2C_PinConfig);
}

void GPIO_Config(void){
    GPIOA_PCLK_EN();
    GPIOB_PCLK_EN();
    GPIOC_PCLK_EN();

    /* PA5 Output */
    GPIOA->MODER &= ~(1 << 10);
    GPIOA->MODER |= 1 << 10;

    /* PB8 input */
    GPIOB->MODER &= ~(0x3 << 16);
    GPIOB->PUPDR &= ~(0x3 << 16);
    GPIOB->PUPDR |= (GPIO_PUPDR_PD << 16);
}

void I2C_Config(void){
    I2CHandle.pI2C                          = I2C1;
    I2CHandle.I2C_Config.I2C_ACK_Control    = I2C_ACK_ENABLE;
    I2CHandle.I2C_Config.I2C_SCLSpeed       = I2C_SCL_SPEED_STANDARD;
    I2CHandle.I2C_Config.I2C_Device_Address = 0x21;
    I2CHandle.RxLen                         = 5;
    I2CHandle.Sr                            = 0;
    I2CHandle.DevAddr                       = 0x3;
    I2CHandle.TxRxState                     = I2C_FREE;

    I2C_Init(&I2CHandle);
}

void MasterSend(void){
    if (BSP_BTN_PRESSED) {
        GPIOA->ODR |= 1 << 5;
        I2C_MasterSendata(&I2CHandle, (uint8_t *)&greeting, 5, 0x3);
        while (BSP_BTN_PRESSED);
    }
    else if (BTN2) {
        GPIOA->ODR |= 1 << 5;
        I2C_MasterSendata(&I2CHandle, (uint8_t *)&goodbye, 5, 0x3);
        while (BTN2);
    }
    GPIOA->ODR &= ~(1 << 5);
}

void MasterReceive(void){
    if(BSP_BTN_PRESSED){
        /* read i2c */
        call_count++;
        while (I2C_MasterReceiveDataIT(&I2CHandle, rxBuffer, 5, 0x3, 0) != I2C_FREE){
            delay_ms(50);
        }

        /* Toggle if string is match */
        strcmp((char *)I2CData.pRxBuffer, "ABCDE") ? LED_TOGGLE() : LED_OFF();

        /* wait til button up */
        while(BSP_BTN_PRESSED);
    }
}

void I2C1_EV_IRQHandler(void){
    ev_count++;
    I2C_EV_IRQHandler(&I2CHandle);
}

int main(void)
{
    GPIO_Config();
    I2C_Pin_Config();
    I2C_Config();

    NVIC_EnableIRQ(I2C1_EV_IRQ);

    /* Enable I2C1 */
    I2C1->CR1 |= 1;
    I2CData.pRxBuffer = (uint8_t*)0;

    /* Infinite loop */
    while (1) {
        MasterReceive();
        LED_TOGGLE();
        delay_ms(100);
    }
    return 1;
}
