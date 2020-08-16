#ifndef KERNEL_ISR_H
#define KERNEL_ISR_H

#include <stdint.h>

struct isr_registers_struct {
//    uint32_t ds;                  // Data segment selector
//    uint32_t edi, esi, ebp, ignored_esp, ebx, edx, ecx, eax; // Pushed by pusha.
//    uint32_t int_no, err_code;    // Interrupt number and error code (if applicable)
//    uint32_t eip, cs, eflags, esp, ss; // Pushed by the processor automatically.
    uint32_t gs, fs, es, ds;
    uint32_t edi, esi, ebp, ignored_esp, ebx, edx, ecx, eax;
    uint32_t int_no, err_code;
    uint32_t eip, cs, eflags, esp, ss;
} __attribute__((packed));
typedef struct isr_registers_struct registers_t;

typedef void (*isr_t)(registers_t*);

void init_isr();

#endif /* KERNEL_ISR_H */
