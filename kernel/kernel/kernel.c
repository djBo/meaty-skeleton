#include <stdio.h>

#include <kernel/multiboot.h>
#include <kernel/tty.h>

void kernel_main(multiboot_info_t *mb_ptr) {
	terminal_initialize();
	printf("Hello, kernel World!\n");
}
