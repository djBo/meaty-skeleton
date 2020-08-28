#include <kernel/font.h>

extern uint32_t font;
static font_t *font_ptr = (font_t*) &font;

uint8_t* font_glyph(uint8_t c) {
	return (uint8_t*) (&(font_ptr->data) + (int) (c - 0x20) * font_ptr->height);
}

uint8_t font_height() {
	return font_ptr->height;
}

uint8_t font_width() {
	return font_ptr->width;
}
