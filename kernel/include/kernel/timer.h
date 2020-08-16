#ifndef KERNEL_TIMER_H
#define KERNEL_TIMER_H

#include <stdint.h>

void init_timer();
void timer_wait(uint32_t ticks);

#endif /* KERNEL_TIMER_H */
