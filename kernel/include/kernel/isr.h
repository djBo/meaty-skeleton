#ifndef KERNEL_ISR_H
#define KERNEL_ISR_H

#include <stdint.h>

#define ISR0 0
#define ISR1 1
#define ISR2 2
#define ISR3 3
#define ISR4 4
#define ISR5 5
#define ISR6 6
#define ISR7 7
#define ISR8 8
#define ISR9 9
#define ISR10 10
#define ISR11 11
#define ISR12 12
#define ISR13 13
#define ISR14 14
#define ISR15 15
#define ISR16 16
#define ISR17 17
#define ISR18 18
#define ISR19 19
#define ISR20 20
#define ISR21 21
#define ISR22 22
#define ISR23 23
#define ISR24 24
#define ISR25 25
#define ISR26 26
#define ISR27 27
#define ISR28 28
#define ISR29 29
#define ISR30 30
#define ISR31 31

struct isr_registers_struct {
	uint32_t gs, fs, es, ds;
	uint32_t edi, esi, ebp, ignored_esp, ebx, edx, ecx, eax;
	uint32_t int_no, err_code;
	uint32_t eip, cs, eflags, esp, ss;
} __attribute__((packed));
typedef struct isr_registers_struct registers_t;

typedef void (*isr_t)(registers_t*);

void init_isr();
void install_service_routine(uint8_t isr, isr_t handler);

#endif /* KERNEL_ISR_H */
