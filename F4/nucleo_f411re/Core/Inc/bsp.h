#ifndef BSP_H
#define BSP_H

#define LED_ON()        (GPIOA->ODR |= (1 << 5))
#define LED_OFF()       (GPIOA->ODR &= ~(1 << 5))
#define LED_TOGGLE()    (GPIOA->ODR ^= (1 << 5))


#endif /* end of include guard: BSP_H */
