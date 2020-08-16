#include <stdio.h>

#include <kernel/multiboot.h>
#include <kernel/gdt.h>
#include <kernel/idt.h>
#include <kernel/isr.h>
#include <kernel/irq.h>
#include <kernel/tty.h>

void kernel_main(multiboot_info_t *mb_ptr) {
    init_gdt();
    init_idt();
    init_isr();
    init_irq();
	terminal_initialize();
	printf("Hello, %s World %d %x !\n", "kernel", 123, 123);

	__asm__ __volatile__ ("sti");

	for (int i = 10; i >= 0; i--) {
	    printf("%d %d\n", i, 100/i);
	}

	for(;;) {
        __asm__ __volatile__ ("hlt");
    }
}
