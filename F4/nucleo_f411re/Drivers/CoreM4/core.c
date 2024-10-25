#include "stdint.h"
#include "core.h"

void NVIC_EnableIRQ(uint32_t irq_num) {
    // Calculate the correct ISER register and bit position
    uint32_t iser_index = irq_num / 32;      // Each ISER controls 32 interrupts
    uint32_t iser_bit = irq_num % 32;        // Position of the interrupt within ISER
    
    // Enable the IRQ in the appropriate ISER
    *((volatile uint32_t *)(NVIC_ISER_BASE + iser_index * 4)) = (1 << iser_bit);
}
