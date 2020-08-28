#ifndef ARCH_I386_VGA_H
#define ARCH_I386_VGA_H

#include <stdint.h>

enum vga_color {
	VGA_COLOR_BLACK = 0,
	VGA_COLOR_BLUE = 1,
	VGA_COLOR_GREEN = 2,
	VGA_COLOR_CYAN = 3,
	VGA_COLOR_RED = 4,
	VGA_COLOR_MAGENTA = 5,
	VGA_COLOR_BROWN = 6,
	VGA_COLOR_LIGHT_GREY = 7,
	VGA_COLOR_DARK_GREY = 8,
	VGA_COLOR_LIGHT_BLUE = 9,
	VGA_COLOR_LIGHT_GREEN = 10,
	VGA_COLOR_LIGHT_CYAN = 11,
	VGA_COLOR_LIGHT_RED = 12,
	VGA_COLOR_LIGHT_MAGENTA = 13,
	VGA_COLOR_LIGHT_BROWN = 14,
	VGA_COLOR_WHITE = 15,
};

// https://en.wikipedia.org/wiki/Video_Graphics_Array#Color_palette
enum color {
	COLOR_BLACK = 0x00000000,
	COLOR_BLUE = 0x000000AA,
	COLOR_GREEN = 0x0000AA00,
	COLOR_CYAN = 0x0000AAAA,
	COLOR_RED = 0x00AA0000,
	COLOR_MAGENTA = 0x00AA00AA,
	COLOR_BROWN = 0x00AA5000,
	COLOR_LIGHT_GREY = 0x00AAAAAA,
	COLOR_DARK_GREY = 0x00555555,
	COLOR_LIGHT_BLUE = 0x005555FF,
	COLOR_LIGHT_GREEN = 0x0055FF55,
	COLOR_LIGHT_CYAN = 0x0055FFFF,
	COLOR_LIGHT_RED = 0x00FF5555,
	COLOR_LIGHT_MAGENTA = 0x00FF55FF,
	COLOR_YELLOW = 0x00FFFF55,
	COLOR_WHITE = 0x00FFFFFF,
};

static const int colors[] = {
	COLOR_BLACK,
	COLOR_BLUE,
	COLOR_GREEN,
	COLOR_CYAN,
	COLOR_RED,
	COLOR_MAGENTA,
	COLOR_BROWN,
	COLOR_LIGHT_GREY,
	COLOR_DARK_GREY,
	COLOR_LIGHT_BLUE,
	COLOR_LIGHT_GREEN,
	COLOR_LIGHT_CYAN,
	COLOR_LIGHT_RED,
	COLOR_LIGHT_MAGENTA,
	COLOR_YELLOW,
	COLOR_WHITE,
};

static const int num_colors = sizeof(colors) / sizeof(int);

static inline uint8_t vga_entry_color(enum vga_color fg, enum vga_color bg) {
	return fg | bg << 4;
}

static inline uint16_t vga_entry(unsigned char uc, uint8_t color) {
	return (uint16_t) uc | (uint16_t) color << 8;
}

#endif
