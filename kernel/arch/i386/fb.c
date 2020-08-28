#include <kernel/fb.h>
#include <kernel/font.h>
#include <kernel/kernel.h>
#include <kernel/timer.h>

#include <string.h>

#include "vga.h"

uint64_t fb_addr = 0;
uint32_t fb_pitch;
uint32_t fb_width;
uint32_t fb_height;
uint8_t fb_bpp;
uint8_t fb_type;

static uint32_t bg_color = 0;
static uint32_t fg_color = 0xFFFFFF;

static uint32_t chr_x = 0, chr_y = 0;
static uint32_t max_x = 0, max_y = 0;
static uint8_t cursor = 0;

static void fb_put(unsigned int idx, unsigned char r, unsigned char g, unsigned char b) {
	unsigned char *screen = (unsigned char*) (uint32_t) fb_addr;
	screen[idx] = b;
	screen[idx + 1] = g;
	screen[idx + 2] = r;
}

void fb_plot(unsigned int x, unsigned int y, unsigned int color) {
	// Ensure we remain in the boundaries of our pixel space to avoid Page Fault exceptions.
	if (x < fb_width && y < fb_height) {
		unsigned int idx = x * (fb_bpp / 8) + y * fb_pitch;
		unsigned char r = (color >> 16) & 255;
		unsigned char g = (color >> 8) & 255;
		unsigned char b = color & 255;
		fb_put(idx, r, g, b);
	}
}

//void fb_rect(unsigned int x, unsigned int y, unsigned int w, unsigned int h, unsigned int color) {
//    for (int i = 0; i < w; i++) {
//        for (int j = 0; j < h; j++) {
//            fb_plot(x + i, y + j, color);
//        }
//    }
//}

void fb_rect(unsigned int x, unsigned int y, unsigned int w, unsigned int h, unsigned int color) {
	// Ensure we remain in the boundaries of our pixel space to avoid Page Fault exceptions.
	if (x >= fb_width || x + w >= fb_width) {
		w = fb_width - x;
	}
	if (y >= fb_height || y + h >= fb_height) {
		h = fb_height - y;
	}

	unsigned int bpp = fb_bpp / 8;
	unsigned int idx = x * bpp + y * fb_pitch;
	unsigned char r = (color >> 16) & 255;
	unsigned char g = (color >> 8) & 255;
	unsigned char b = color & 255;

	for (unsigned int j = 0; j < h; j++) {
		for (unsigned int i = 0; i < w; i++) {
			fb_put(idx + i * bpp, r, g, b);
		}
		idx += fb_pitch;
	}
}

void fb_char(unsigned char c, unsigned int x, unsigned int y, unsigned int fgcolor, unsigned int bgcolor) {
	unsigned char *glyph = font_glyph(c);

	for (int cy = 0; cy < font_height(); cy++) {
		for (int cx = 0; cx < font_width(); cx++) {
			if (glyph[cy] & font_mask[cx]) {
				fb_plot(x + cx, y + cy, fgcolor);
			} else {
				fb_plot(x + cx, y + cy, bgcolor);
			}
		}
	}
}

/****************************************************/

void fb_putch(uint8_t c) {
	if (c >= ' ' && c < 128) {
		fb_char(c, chr_x, chr_y, fg_color, bg_color);
		chr_x += font_width();
	} else {
		switch (c) {
		case 0x08:
			if (cursor == 1) {
				fb_char(' ', chr_x, chr_y, fg_color, bg_color);
			}
			if (chr_x > 0) {
				chr_x -= font_width();
				fb_char(' ', chr_x, chr_y, fg_color, bg_color);
			}
			if (cursor == 1) {
				fb_char(128, chr_x, chr_y, fg_color, bg_color);
			}
			break;
		case 0x09:
			// Increment 8
			break;
		case '\r':
			chr_x = 0;
			break;
		case '\n':
			if (cursor == 1) {
				fb_char(' ', chr_x, chr_y, fg_color, bg_color);
			}
			chr_x = 0;
			chr_y += font_height();
			if (cursor == 1) {
				fb_char(128, chr_x, chr_y, fg_color, bg_color);
			}
		}
	}

	// Did we reach the end of the line?
	if (chr_x >= (max_x * font_width())) {
		chr_x = 0;
		chr_y += font_height();
	}

	// Do we need to scroll?
	if (chr_y >= fb_height) {
		// Ugh!!
	}

    /* Any character greater than and including a space, is a
    *  printable character. The equation for finding the index
    *  in a linear chunk of memory can be represented by:
    *  Index = [(y * width) + x] */
//    else if(c >= ' ')
//    {
//        where = textmemptr + (csr_y * 80 + csr_x);
//        *where = c | att; /* Character AND attributes: color */
//        csr_x++;
//    }
//
//    /* If the cursor has reached the edge of the screen's width, we
//    *  insert a new line in there */
//    if(csr_x >= 80)
//    {
//        csr_x = 0;
//        csr_y++;
//    }
//
//    /* Scroll the screen if needed, and finally move the cursor */
//    scroll();
//    move_csr();
}

void fb_clear() {
	cursor = 0;
	chr_x = 0;
	chr_y = 0;
	memset((void*) (uint32_t) fb_addr, 0, fb_width * fb_height * (fb_bpp / 8));
}

void fb_cursor() {
	if (cursor == 1) {
		cursor = 0;
		fb_char(' ', chr_x, chr_y, fg_color, bg_color);
	} else {
		cursor = 1;
		fb_char(128, chr_x, chr_y, fg_color, bg_color);
	}
}

void fb_color(uint32_t foreground, uint32_t background) {
	fg_color = foreground;
	bg_color = background;
}

void fb_set(uint64_t addr) {
	fb_addr = addr;
}

void init_fb(multiboot_info_t *mb_ptr) {
	if (CHECK_FLAG(mb_ptr->flags, MULTIBOOT_INFO_FRAMEBUFFER_INFO)) {
		fb_addr = mb_ptr->framebuffer_addr;
		fb_pitch = mb_ptr->framebuffer_pitch;
		fb_width = mb_ptr->framebuffer_width;
		fb_height = mb_ptr->framebuffer_height;
		fb_bpp = mb_ptr->framebuffer_bpp;
		fb_type = mb_ptr->framebuffer_type;

		fb_color(colors[VGA_COLOR_LIGHT_GREY], colors[VGA_COLOR_BLACK]);

		max_x = fb_width / font_width();
		max_y = fb_height / font_height();
		timer_func = fb_cursor;
	}
}
