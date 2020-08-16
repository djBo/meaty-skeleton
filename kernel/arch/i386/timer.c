#include <kernel/timer.h>
#include <kernel/sysio.h>
#include <kernel/irq.h>

#include <stdlib.h>

volatile unsigned long timer_ticks = 0;

void timer_handler(__attribute__((unused)) registers_t *regs) {
    timer_ticks++;
    srand(rand());
}

void timer_phase(int hz) {
    int divisor = 1193180 / hz;
    outportb(0x43, 0x36);
    outportb(0x40, divisor & 0xFF);
    outportb(0x40, divisor >> 8);
}

void timer_wait(uint32_t ticks) {
    unsigned int eticks;

    eticks = timer_ticks + ticks;
    while (timer_ticks < eticks) {
        __asm__ __volatile__ ("sti//hlt//cli");
    }
}

void init_timer() {
    timer_phase(100);
    install_interrupt_handler(0, timer_handler);
}
