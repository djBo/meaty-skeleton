#include <stdio.h>

#include <kernel/multiboot.h>
#include <kernel/gdt.h>
#include <kernel/idt.h>
#include <kernel/isr.h>
#include <kernel/irq.h>
#include <kernel/keyboard.h>
#include <kernel/timer.h>
#include <kernel/tty.h>

void kernel_main(multiboot_info_t *mb_ptr) {
    terminal_initialize();
    init_gdt();
    init_idt();
    init_isr();
    init_irq();
    init_timer();
    init_keyboard();

    printf("Hello, %s World %d %x !\n", "kernel", 123, 123);

	__asm__ __volatile__ ("sti");

	for(;;) {
        __asm__ __volatile__ ("hlt");
        unsigned char c = getchar();
        if (c > 0) {
            if (c == 27) {
                terminal_initialize();
            } else {
                putchar(c);
            }
        }
    }

}
