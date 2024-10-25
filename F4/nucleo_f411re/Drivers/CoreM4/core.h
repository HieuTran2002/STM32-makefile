#ifndef COREM4_H
#define COREM4_H

#define NVIC_ISER_BASE      (0xE000E100)  // NVIC ISER base address

// Function to enable the IRQ number in NVIC
void NVIC_EnableIRQ(uint32_t irq_num);



#endif /* end of include guard: COREM4_H */
