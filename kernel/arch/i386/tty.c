#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include <kernel/fb.h>
#include <kernel/kernel.h>
#include <kernel/tty.h>
#include <kernel/sysio.h>

#include "vga.h"

static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 25;
static uint16_t *const VGA_MEMORY = (uint16_t*) 0xB8000;

static size_t terminal_row;
static size_t terminal_column;
static uint8_t terminal_color;
static uint16_t *terminal_buffer;

static uint8_t term_graphics = 0;

static void terminal_movecursor() {
	const uint16_t pos = terminal_row * VGA_WIDTH + terminal_column;
	outportb(0x3D4, 14);
	outportb(0x3D5, pos >> 8);
	outportb(0x3D4, 15);
	outportb(0x3D5, pos);
}

static void terminal_scroll() {
	const uint16_t space = 0x20 | (terminal_color << 8);
	if (terminal_row >= VGA_HEIGHT) {
		for (uint32_t i = 0; i < (VGA_HEIGHT - 1) * VGA_WIDTH; i++) {
			terminal_buffer[i] = terminal_buffer[i + VGA_WIDTH];
		}
		for (uint32_t i = (VGA_HEIGHT - 1) * VGA_WIDTH;
				i < VGA_HEIGHT * VGA_WIDTH; i++) {
			terminal_buffer[i] = space;
		}
		terminal_row = VGA_HEIGHT - 1;
	}
}

void terminal_clear() {
	if (term_graphics) {
		fb_clear();
	} else {
		terminal_row = 0;
		terminal_column = 0;
		for (size_t y = 0; y < VGA_HEIGHT; y++) {
			for (size_t x = 0; x < VGA_WIDTH; x++) {
				const size_t index = y * VGA_WIDTH + x;
				terminal_buffer[index] = vga_entry(' ', terminal_color);
			}
		}
		terminal_movecursor();
	}
}

static void terminal_initialize(void) {
	terminal_buffer = VGA_MEMORY;
	terminal_setcolor(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
	terminal_clear();
}

void terminal_setcolor(uint8_t fg, uint8_t bg) {
	if (term_graphics) {
		fb_color(colors[fg], colors[bg]);
	} else {
		terminal_color = vga_entry_color(fg, bg);
	}
}

static void terminal_putentryat(unsigned char c, uint8_t color, size_t x, size_t y) {
	const size_t index = y * VGA_WIDTH + x;
	terminal_buffer[index] = vga_entry(c, color);
}

void terminal_putchar(char c) {
	unsigned char uc = c;
	if (term_graphics) {
		fb_putch(uc);
	} else {
		if (c == 0x08 && terminal_column) {
			terminal_column--;
		} else if (c == 0x09) {
			terminal_column = (terminal_column + 8) & ~(8 - 1);
		} else if (c == '\n') {
			terminal_column = 0;
			terminal_row++;
		} else if (c >= ' ') {
			terminal_putentryat(uc, terminal_color, terminal_column, terminal_row);
			terminal_column++;
		}

		if (terminal_column >= VGA_WIDTH) {
			terminal_column = 0;
			terminal_row++;
		}

		terminal_scroll();
		terminal_movecursor();
	}
}

void terminal_write(const char *data, size_t size) {
	for (size_t i = 0; i < size; i++)
		terminal_putchar(data[i]);
}

void terminal_writestring(const char *data) {
	terminal_write(data, strlen(data));
}

void init_term(multiboot_info_t *mb_ptr) {
	if (CHECK_FLAG(mb_ptr->flags, MULTIBOOT_INFO_FRAMEBUFFER_INFO)) {
		if (mb_ptr->framebuffer_type == 2) {
			terminal_initialize();
		} else {
			term_graphics = 1;
			init_fb(mb_ptr);
		}
	} else {
		terminal_initialize();
	}
}
