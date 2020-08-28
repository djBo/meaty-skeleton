#include <kernel/paging.h>
#include <kernel/isr.h>

#include <stdio.h>
#include <stdlib.h>

//#define MAP_FB_TO_4MB

// Straight from https://wiki.osdev.org/Setting_Up_Paging

static uint32_t page_directory[1024] __attribute__((aligned(4096)));
static uint32_t first_page_table[1024] __attribute__((aligned(4096)));
static uint32_t fb_page_table[1024] __attribute__((aligned(4096)));

extern void paging_enable(void);
extern void pd_load(uint32_t*);

static void init_pd() {
	//set each entry to not present
	for (int i = 0; i < 1024; i++) {
		// This sets the following flags to the pages:
		// - Supervisor: Only kernel-mode can access them
		// - Write Enabled: It can be both read from and written to
		// - Not Present: The page table is not present
		page_directory[i] = 0x00000002;
	}
}

static void init_fpt() {
	// holds the physical address where we want to start mapping these pages to.
	// in this case, we want to map these pages to the very beginning of memory.

	//we will fill all 1024 entries in the table, mapping 4 megabytes
	for (unsigned int i = 0; i < 1024; i++) {
		// As the address is page aligned, it will always leave 12 bits zeroed.
		// Those bits are used by the attributes ;)
		first_page_table[i] = (i * 0x1000) | 3; // attributes: supervisor level, read/write, present.
	}
	page_directory[0] = ((unsigned int) first_page_table) | 3;
}

// Initialize enough tables at the right location to map the framebuffer memory region.
// Note that we only have a single table, only capable of mapping 4Mb.
static void init_fbpt(multiboot_info_t *mb_ptr) {
	//	// TODO: Read these values from multiboot
	//	static const uint32_t fb_start = 0xFD000000;
	//	static const uint32_t fb_width = 800;
	//	static const uint32_t fb_height = 600;
	//	static const uint32_t fb_bpp = 32;

	if (((mb_ptr->flags & MULTIBOOT_INFO_FRAMEBUFFER_INFO) == MULTIBOOT_INFO_FRAMEBUFFER_INFO) && (mb_ptr->framebuffer_type < 2)) {
		// At what directory does this table go?
		unsigned int page_index = (mb_ptr->framebuffer_addr / 0x1000) / 0x400;

		unsigned int fb_size = mb_ptr->framebuffer_width * mb_ptr->framebuffer_height * (mb_ptr->framebuffer_bpp / 8);
		unsigned int fb_blocks = div2(fb_size, 0x1000);

		__builtin_printf("At: %u, size: %u, blocks: %u\n", page_index, fb_size, fb_blocks);
		__builtin_printf("Pd: 0x%08X, Kpt: 0x%08X, FBpt: 0x%08X\n", (unsigned) page_directory, (unsigned) first_page_table, (unsigned) fb_page_table);

		for (unsigned int i = 0; i < fb_blocks; i++) {
			// As the address is page aligned, it will always leave 12 bits zeroed.
			// Those bits are used by the attributes ;)
			fb_page_table[i] = (mb_ptr->framebuffer_addr + (i * 0x1000)) | 3; // attributes: supervisor level, read/write, present.
		}
#ifndef MAP_FB_TO_4MB
		page_directory[page_index] = ((unsigned int) fb_page_table) | 3;
#else
		page_directory[1] = ((unsigned int) fb_page_table) | 3;
#endif
	}

}

void page_fault_handler(__attribute__((unused))  registers_t *regs) {
	// The faulting address is stored in the CR2 register.
	uint32_t faulting_addr;
	__asm__ __volatile__ ("mov %%cr2, %0" : "=r" (faulting_addr));

	// The error code gives us details of what happened.
	int present = !(regs->err_code & 0x1); // Page not present
	int rw = regs->err_code & 0x2;           // Write operation?
	int us = regs->err_code & 0x4;           // Processor was in user-mode?
	int reserved = regs->err_code & 0x8; // Overwritten CPU-reserved bits of page entry?
	//int id = regs->err_code & 0x10;          // Caused by an instruction fetch?

	// Output an error message.
	printf("Page Fault exception ( ");
	if (present) {
		printf("present ");
	}
	if (rw) {
		printf("read-only ");
	}
	if (us) {
		printf("user-mode ");
	}
	if (reserved) {
		printf("reserved ");
	}
	printf(") at 0x%X, halting system\n", faulting_addr);

	for (;;) {
		__asm__ __volatile__ ("hlt");
	}
}

void init_paging(multiboot_info_t *mb_ptr) {
	init_pd();
	init_fpt();
	init_fbpt(mb_ptr);
	install_service_routine(14, page_fault_handler);
	pd_load(page_directory);
	paging_enable();
#ifdef MAP_FB_TO_4MB
	fb_set(0x400000);
#endif
}
