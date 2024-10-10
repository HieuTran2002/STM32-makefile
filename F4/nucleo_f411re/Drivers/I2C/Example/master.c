#include "stm32f411xx.h"
#include "stm32f411xx_clock.h"
#include "stm32f411xx_gpio_driver.h"
#include "stm32f411xx_i2c_driver.h"
#include <stdint.h>
#include "bsp.h"
#include "string.h"


#define BTN2     ((GPIOB->IDR >> 8) & 1)
#define USER_BTN !((GPIOC->IDR >> 13) & 1)

char greeting[] = "Good morning VietNam\n";
char goodbye[] = "Fuck VietNam\n";

/* I2C data buffer */
typedef struct {
    uint8_t* TxBuffer;
    uint8_t* RxBuffer;
} I2C_Data;
I2C_Data I2C_data = {0};


I2C_Handle_Type I2C_Handle;

/* 
 * PB6 SCL
 * PB7 SDA
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
    I2C_Handle.pI2C                          = I2C1;
    I2C_Handle.I2C_Config.I2C_ACK_Control    = I2C_ACK_ENABLE;
    I2C_Handle.I2C_Config.I2C_SCLSpeed       = I2C_SCL_SPEED_STANDARD;
    I2C_Handle.I2C_Config.I2C_Device_Address = 0x15;

    I2C_Init(&I2C_Handle);
}

void MasterSend(void){
    if (USER_BTN) {
        GPIOA->ODR |= 1 << 5;
        I2C1->CR1 |= 1;
        I2C_MasterSendata(&I2C_Handle, (uint8_t *)&greeting, 5, 0x3);
        while (USER_BTN);
    }
    else if (BTN2) {
        GPIOA->ODR |= 1 << 5;
        I2C1->CR1 |= 1;
        I2C_MasterSendata(&I2C_Handle, (uint8_t *)&goodbye, 5, 0x3);
        while (BTN2);
    }
    GPIOA->ODR &= ~(1 << 5);
}


void MasterReceive(void){

    
    if(USER_BTN){
        I2C_MasterReadData(&I2C_Handle, I2C_data.RxBuffer, 5, 0x3);
        if (strcmp((char *)I2C_data.RxBuffer, "ABCDE")) {
            LED_TOGGLE();
        }
        else{
            LED_OFF();
        }
        while(USER_BTN);
    }
}
int main(void)
{
    GPIO_Config();
    I2C_Pin_Config();
    I2C_Config();
    
    /* Enable I2C1 */
    I2C1->CR1 |= 1;

    /* Infinite loop */
    while (1) {
        MasterReceive();
        delay_ms(100);
    }
    return 1;
}
