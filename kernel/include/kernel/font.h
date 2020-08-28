#ifndef KERNEL_FONT_H
#define KERNEL_FONT_H

#include <stdint.h>

struct font_struct {
	uint64_t id;
	uint8_t height;
	uint8_t width;
	uint8_t data;
} __attribute__((packed));
typedef struct font_struct font_t;

static const uint32_t font_mask[8] = { 128, 64, 32, 16, 8, 4, 2, 1 };

uint8_t* font_glyph(uint8_t c);
uint8_t font_height();
uint8_t font_width();

#endif /* KERNEL_FONT_H */
