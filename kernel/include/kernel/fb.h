#ifndef KERNEL_FB_H
#define KERNEL_FB_H

#include <stdint.h>
#include <kernel/multiboot.h>

extern uint32_t fb_width;
extern uint32_t fb_height;

void fb_clear();
void fb_color(uint32_t foreground, uint32_t background);
void fb_cursor();
void fb_putch(uint8_t c);

void fb_plot(unsigned int x, unsigned int y, unsigned int color);
void fb_rect(unsigned int x, unsigned int y, unsigned int w, unsigned int h, unsigned int color);
void fb_char(unsigned char c, unsigned int x, unsigned int y, unsigned int fgcolor, unsigned int bgcolor);

//void fb_set(uint64_t addr);
void init_fb(multiboot_info_t *mb_ptr);

#endif /* KERNEL_FONT_H */
