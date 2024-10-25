#ifndef BSP_H
#define BSP_H

#include "stm32g431xx.h"

#define LED_PIN         6
#define LED_PORT        GPIOC
#define LED_ON()        (GPIOC->ODR |= (1 << 6))
#define LED_OFF()       (GPIOC->ODR &= ~(1 << 6))

#define USER_BTN_PIN    13
#define USER_BTN_PORT   GPIOC

#define BTN_PRESSED     (((GPIOC->IDR) >> 13) & 1)

#define LED_TOGGLE()    (LED_PORT->ODR ^= 1 << LED_PIN)
#endif /* end of include guard: BSP_H */
