#ifndef KERNEL_TIMER_H
#define KERNEL_TIMER_H

#include <stdint.h>

extern void (*timer_func)();
void timer_wait(uint32_t ticks);

void init_timer();

#endif /* KERNEL_TIMER_H */
