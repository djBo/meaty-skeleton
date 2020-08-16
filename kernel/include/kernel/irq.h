#ifndef KERNEL_IRQ_H
#define KERNEL_IRQ_H

#include <stdint.h>
#include <kernel/isr.h>

void init_irq();
void install_interrupt_handler(uint8_t irq, isr_t handler);

#endif /* KERNEL_IRQ_H */
