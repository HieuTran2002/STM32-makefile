#include "stdint.h"
#include "stm32f103xb.h"
#include "stm32f1xx_clock.h"
#include "stm32f103xx_gpio_drivers.h"
#include "bsp.h"

#define BTN_PRESSED     !(GPIOB->IDR & (1 << 4))

int main(void)
{
    LED_Init();
    while (1) {
        GPIOB->BSRR |= (1 << (LED_PIN + 16 * (!BTN_PRESSED)));
        delay_ms(10);
    }
    delay_ms(10);
    return 1;
}
