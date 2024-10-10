#include "bsp.h"
#include "stm32g431_gpio_drivers.h"
#include "stm32g431xx.h"
#include "stm32g431xx_clock.h"
#include "stm32g4xx_i2c_drivers.h"
#include <stdint.h>

I2C_Handle_Type I2C2_Handle;

typedef struct {
    uint8_t* TxBuffer;
    uint8_t* RxBuffer;
} I2C_Data;

I2C_Data I2C_data = {0};


void GPIO_Config(void){
    GPIO_Handle_Type GPIO_Handle;
    
    /* LED */
    GPIO_Handle.pGPIO                 = GPIOC;
    GPIO_Handle.GPIO_Config.PinNumber = 6;
    GPIO_Handle.GPIO_Config.PinMode   = GPIO_MODER_OUTPUT;
    GPIO_Handle.GPIO_Config.Pull      = GPIO_PUPDR_PD;
    GPIO_Handle.GPIO_Config.OType     = GPIO_OTYPE_PP;
    GPIO_Handle.GPIO_Config.Speed     = GPIO_OSPEED_FAST;
    GPIO_Init(&GPIO_Handle);

    /* USER BTN */
    GPIO_Handle.GPIO_Config.PinNumber = 13;
    GPIO_Handle.GPIO_Config.PinMode   = GPIO_MODER_INPUT;
    GPIO_Init(&GPIO_Handle);
}

void I2C2_GPIO_Config(void){
    /* 
     * PA8 SDA
     * PA9 SCL
     */
    GPIO_Handle_Type I2C2_Pin;
     
    I2C2_Pin.pGPIO               = GPIOA;
    I2C2_Pin.GPIO_Config.PinMode = GPIO_MODER_ALTFNC;
    I2C2_Pin.GPIO_Config.OType   = GPIO_OTYPE_OD;
    I2C2_Pin.GPIO_Config.Pull    = GPIO_PUPDR_PU;
    I2C2_Pin.GPIO_Config.AltFunc = 4;

    I2C2_Pin.GPIO_Config.PinNumber = 8;
    GPIO_Init(&I2C2_Pin);

    I2C2_Pin.GPIO_Config.PinNumber = 9;
    GPIO_Init(&I2C2_Pin);
}

void I2C2_Config(void){
    I2C2_Handle.pI2C = I2C2;
    I2C2_Handle.I2C_Config.I2C_SCLSpeed = I2C_SCL_SPEED_STANDARD;
    I2C2_Handle.I2C_Config.I2C_NACK_Control = 0;
    I2C2_Handle.I2C_Config.I2C_Device_Address = 0x3;

    I2C_Init(&I2C2_Handle);
}

void SlaveRead(void){
    I2C_SlaveReceiveData(&I2C2_Handle, (uint8_t *)"Ohiooooo", 5);
    if      (*(I2C_data.RxBuffer) == 'G') {
        LED_PORT->ODR |= (1 << LED_PIN);
    }
    else if (*(I2C_data.RxBuffer) == 'F') {
        LED_PORT->ODR &= ~(1 << LED_PIN);
    }
}

void SlaveSend(void){
    I2C_SlaveSendata(&I2C2_Handle, (uint8_t *)"ABCDE", 5);
}

int main(void)
{
    GPIO_Config();
    I2C2_GPIO_Config();
    I2C2_Config();
    I2C_data.RxBuffer = &(uint8_t){0};
    I2C_data.TxBuffer = (uint8_t *)"Ohio..........!";

    I2C2->CR1 |= 1;

    while (1) {
        SlaveSend();
        delay_ms(1);
    }
    return 1;
}
