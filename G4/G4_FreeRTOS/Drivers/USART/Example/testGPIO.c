#include "bsp.h"
#include "stm32g431xx.h"
#include "stm32g431xx_clock.h"
#include "stm32g4xx.h"

void GPIO_Config(){
    RCC->APB1ENR1 |= (1 << RCC_APB1ENR1_USART2EN_Pos);

    RCC->AHB2ENR |= (1 << RCC_AHB2ENR_GPIOAEN_Pos);
    RCC->AHB2ENR |= (1 << RCC_AHB2ENR_GPIOBEN_Pos);
    RCC->AHB2ENR |= (1 << RCC_AHB2ENR_GPIOCEN_Pos);

    GPIOA->MODER &= ~(0x3 << 4);
    GPIOA->MODER &= ~(0x3 << 6);

    GPIOA->MODER |= (1 << 4);
    GPIOA->MODER |= (1 << 6);

    GPIOC->MODER &= ~(0x3 << 12);
    GPIOC->MODER |=  (1 << 12);
}

int main() {
    GPIO_Config();
    for (;;){
        GPIOA->ODR ^= (1 << 2);
        GPIOA->ODR ^= (1 << 3);
        delay_ms(100);
    }
}
