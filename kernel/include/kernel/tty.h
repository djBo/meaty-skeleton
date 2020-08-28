#ifndef _KERNEL_TTY_H
#define _KERNEL_TTY_H

#include <stddef.h>
#include <stdint.h>
#include <kernel/multiboot.h>

void terminal_clear();
void terminal_putchar(char c);
void terminal_setcolor(uint8_t fg, uint8_t bg);
void terminal_write(const char* data, size_t size);
void terminal_writestring(const char* data);

void init_term(multiboot_info_t *mb_ptr);

#endif
