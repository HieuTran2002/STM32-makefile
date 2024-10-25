#ifndef BSP_H
#define BSP_H

#include "stm32f411xx.h"

#define LED_PIN             5
#define LED_PORT            GPIOA

#define LED_ON()            (GPIOA->ODR |= (1 << 5))
#define LED_OFF()           (GPIOA->ODR &= ~(1 << 5))
#define LED_TOGGLE()        (GPIOA->ODR ^= (1 << 5))

#define BSP_BTN_PRESSED     !((GPIOC->IDR >> 13) & 1)


#endif /* end of include guard: BSP_H */
