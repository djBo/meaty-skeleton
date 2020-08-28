#include <stdio.h>
#include <stdlib.h>

#include <kernel/kernel.h>
#include <kernel/multiboot.h>
#include <kernel/gdt.h>
#include <kernel/idt.h>
#include <kernel/isr.h>
#include <kernel/irq.h>
#include <kernel/keyboard.h>
#include <kernel/timer.h>
#include <kernel/tty.h>
#include <kernel/fb.h>
#include <kernel/paging.h>

#include <kernel/program.h>

//#define TEST_FB

extern uint32_t lsb(uint32_t);
extern uint32_t __mboot;
extern uint32_t __end;

static void debug_flags(multiboot_info_t *mb_ptr) {
	printf("Flags: 0x%X\n", mb_ptr->flags);

	// Check
	if (CHECK_FLAG(mb_ptr->flags, MULTIBOOT_INFO_MEMORY)) {
		printf("Lower: %d, upper: %d\n", mb_ptr->mem_lower, mb_ptr->mem_upper);
	}

	// Check
	if (CHECK_FLAG(mb_ptr->flags, MULTIBOOT_INFO_BOOTDEV)) {
		printf("Booted off: 0x%X\n", mb_ptr->boot_device);
	}

	// Check
	if (CHECK_FLAG(mb_ptr->flags, MULTIBOOT_INFO_CMDLINE)) {
		printf("cmdline: %s\n", (char*) mb_ptr->cmdline);
	}

	// Check
	if (CHECK_FLAG(mb_ptr->flags, MULTIBOOT_INFO_MODS)) {
		printf("Modules: 0x%X, count: %d\n", mb_ptr->mods_addr,
				mb_ptr->mods_count);

		multiboot_module_t *mod;
		unsigned int i;
		for (i = 0, mod = (multiboot_module_t*) mb_ptr->mods_addr;
				i < mb_ptr->mods_count; i++, mod++)
			printf(" mod_start = 0x%X, mod_end = 0x%X, cmdline = %s\n",
					(unsigned) mod->mod_start, (unsigned) mod->mod_end,
					(char*) mod->cmdline);
	}

	if (CHECK_FLAG(mb_ptr->flags, MULTIBOOT_INFO_AOUT_SYMS)) {
		printf("AOUT: 0x%X\n", mb_ptr->u.aout_sym.addr);
	}

	// Check
	if (CHECK_FLAG(mb_ptr->flags, MULTIBOOT_INFO_ELF_SHDR)) {
		printf("ELF: 0x%X, size: %d, count: %d\n", mb_ptr->u.elf_sec.addr,
				mb_ptr->u.elf_sec.size, mb_ptr->u.elf_sec.num);
	}

	// Check
	if (CHECK_FLAG(mb_ptr->flags, MULTIBOOT_INFO_MEM_MAP)) {
		printf("Map: 0x%X, length: %d\n", mb_ptr->mmap_addr,
				mb_ptr->mmap_length);

		multiboot_memory_map_t *mmap;
		for (mmap = (multiboot_memory_map_t*) mb_ptr->mmap_addr;
				(unsigned long) mmap < mb_ptr->mmap_addr + mb_ptr->mmap_length;
				mmap = (multiboot_memory_map_t*) ((unsigned long) mmap
						+ mmap->size + sizeof(mmap->size)))
			printf(" size = 0x%X, base_addr = 0x%X%08X,"
					" length = 0x%X%08X, type = 0x%X\n", (unsigned) mmap->size,
					(unsigned) (mmap->addr >> 32),
					(unsigned) (mmap->addr & 0xffffffff),
					(unsigned) (mmap->len >> 32),
					(unsigned) (mmap->len & 0xffffffff), (unsigned) mmap->type);

	}

	if (CHECK_FLAG(mb_ptr->flags, MULTIBOOT_INFO_DRIVE_INFO)) {
		printf("Drives: 0x%X, size: %d\n", mb_ptr->drives_addr,
				mb_ptr->drives_length);
	}

	if (CHECK_FLAG(mb_ptr->flags, MULTIBOOT_INFO_CONFIG_TABLE)) {
		printf("Config: 0x%X\n", mb_ptr->config_table);
	}

	// Check
	if (CHECK_FLAG(mb_ptr->flags, MULTIBOOT_INFO_BOOT_LOADER_NAME)) {
		printf("Loaded by: %s\n", (char*) mb_ptr->boot_loader_name);
	}

	if (CHECK_FLAG(mb_ptr->flags, MULTIBOOT_INFO_APM_TABLE)) {
		printf("APM: 0x%X\n", mb_ptr->apm_table);
	}

	// Check
	if (CHECK_FLAG(mb_ptr->flags, MULTIBOOT_INFO_VBE_INFO)) {
		printf("VBE: 0x%X & 0x%X, mode: %d\n", mb_ptr->vbe_control_info,
				mb_ptr->vbe_mode_info, mb_ptr->vbe_mode);
	}

	// Check
	if (CHECK_FLAG(mb_ptr->flags, MULTIBOOT_INFO_FRAMEBUFFER_INFO)) {
		printf("FB: 0x%X, pitch: %d, %dx%dx%d, type: %d\n",
				(uint32_t) mb_ptr->framebuffer_addr, mb_ptr->framebuffer_pitch,
				mb_ptr->framebuffer_width, mb_ptr->framebuffer_height,
				mb_ptr->framebuffer_bpp, mb_ptr->framebuffer_type);
	}
}

static void kernel_init(multiboot_info_t *mb_ptr) {
	init_term(mb_ptr);
	init_gdt();
	init_idt();
	init_isr();
	init_irq();
	init_timer();
	init_keyboard();
//#ifndef TEST_FB
	init_paging(mb_ptr);
//#endif
}

#ifdef TEST_FB
extern uint32_t fb_width;
extern uint32_t fb_height;

uint32_t x, max_x;
uint32_t y, max_y;
uint8_t r, g, b;

//http://www.cplusplus.com/reference/climits/
#define LLONG_MAX 9223372036854775807
#define LLONG_MIN -9223372036854775807

//https://stackoverflow.com/a/41871699/553317
double floor(double num) {
	if (num >= LLONG_MAX || num <= LLONG_MIN || num != num) {
		/* handle large values, infinities and nan */
		return num;
	}
	long long n = (long long) num;
	double d = (double) n;
	if (d == num || num >= 0) {
		return d;
	} else {
		return d - 1;
	}
}

double round(double num) {
	return floor(num + 0.5);
}

//https://stackoverflow.com/a/5732390/553317
int map(int input, int input_start, int input_end, int output_start, int output_end) {
	double slope = 1.0 * (output_end - output_start) / (input_end - input_start);
	return output_start + round(slope * (input - input_start));
}

unsigned int color(unsigned int r, unsigned int g, unsigned int b) {
	return
		((r << 16) & 0xFF0000) +
		((g << 8) & 0xFF00) +
		(b & 0xFF);
}
#endif

void kernel_main(multiboot_info_t *mb_ptr, uint32_t stack_ptr) {
	kernel_init(mb_ptr);

	printf("Kernel start: 0x%08X, end: 0x%08X\n", &__mboot, &__end);
	printf("Stack: 0x%X 0x%X\n", stack_ptr, &stack_ptr);
	debug_flags(mb_ptr);

	multiboot_module_t *mod = (multiboot_module_t*) mb_ptr->mods_addr;
	call_module_t program = (call_module_t) mod->mod_start;
	printf("Module: 0x%X\n", program);
	uint32_t retval = program();
	printf("Return: 0x%X\n", retval);

	__asm__ __volatile__ ("sti");

#ifdef TEST_FB
	max_x = fb_width / 4;
	max_y = fb_height / 4;

	printf("max x: %d, max y: %d\n", max_x, max_y);
#endif
	for (;;) {
#ifdef TEST_FB
		x = map(rand(), 0, 32768, 0, max_x);
		y = map(rand(), 0, 32768, 0, max_y);

		r = map(rand(), 0, 32768, 0, 256);
		g = map(rand(), 0, 32768, 0, 256);
		b = map(rand(), 0, 32768, 0, 256);

		fb_rect(x * 4, y * 4, 4, 4, color(r, g, b));
#else
		__asm__ __volatile__ ("hlt");
		unsigned char c = getchar();
		if (c > 0) {
			if (c == 27) {
				terminal_clear();

				// Generate a page fault exception
				uint32_t *ptr = (uint32_t*) 0xA0000000;
				uint32_t do_page_fault = *ptr;
				printf("0x%X\n", do_page_fault);

			} else {
				putchar(c);
			}
		}
#endif
	}

}
