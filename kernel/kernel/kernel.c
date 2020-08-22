#include <stdio.h>

#include <kernel/multiboot.h>
#include <kernel/gdt.h>
#include <kernel/idt.h>
#include <kernel/isr.h>
#include <kernel/irq.h>
#include <kernel/keyboard.h>
#include <kernel/timer.h>
#include <kernel/tty.h>

extern uint32_t __mboot;
extern uint32_t __end;

static void debug_flags(multiboot_info_t *mb_ptr) {
    printf("Flags: 0x%x\n", mb_ptr->flags);

    // Check
    if ((mb_ptr->flags & MULTIBOOT_INFO_MEMORY) == MULTIBOOT_INFO_MEMORY) {
        printf("Lower: %d, upper: %d\n", mb_ptr->mem_lower, mb_ptr->mem_upper);
    }

    // Check
    if ((mb_ptr->flags & MULTIBOOT_INFO_BOOTDEV) == MULTIBOOT_INFO_BOOTDEV) {
        printf("Booted off: 0x%x\n", mb_ptr->boot_device);
    }

    // Check
    if ((mb_ptr->flags & MULTIBOOT_INFO_CMDLINE) == MULTIBOOT_INFO_CMDLINE) {
        printf("cmdline: %s\n", (char *)mb_ptr->cmdline);
    }

    // Check
    if ((mb_ptr->flags & MULTIBOOT_INFO_MODS) == MULTIBOOT_INFO_MODS) {
        printf("Modules: %d\n", mb_ptr->mods_count);
    }

    if ((mb_ptr->flags & MULTIBOOT_INFO_AOUT_SYMS) == MULTIBOOT_INFO_AOUT_SYMS) {
        printf("AOUT: 0x%x\n", mb_ptr->u.aout_sym.addr);
    }

    // Check
    if ((mb_ptr->flags & MULTIBOOT_INFO_ELF_SHDR) == MULTIBOOT_INFO_ELF_SHDR) {
        printf("ELF: 0x%x, size: %d, count: %d\n", mb_ptr->u.elf_sec.addr, mb_ptr->u.elf_sec.size, mb_ptr->u.elf_sec.num);
    }

    // Check
    if ((mb_ptr->flags & MULTIBOOT_INFO_MEM_MAP) == MULTIBOOT_INFO_MEM_MAP) {
        printf("Map: 0x%x, length: %d\n", mb_ptr->mmap_addr, mb_ptr->mmap_length);

        multiboot_memory_map_t *mmap;
        for (mmap = (multiboot_memory_map_t *) mb_ptr->mmap_addr;
                (unsigned long) mmap < mb_ptr->mmap_addr + mb_ptr->mmap_length;
                mmap = (multiboot_memory_map_t *) ((unsigned long) mmap + mmap->size + sizeof (mmap->size)))
            printf (" size = 0x%x, base_addr = 0x%x%08x,"
                            " length = 0x%x%08x, type = 0x%x\n",
                            (unsigned) mmap->size,
                            (unsigned) (mmap->addr >> 32),
                            (unsigned) (mmap->addr & 0xffffffff),
                            (unsigned) (mmap->len >> 32),
                            (unsigned) (mmap->len & 0xffffffff),
                            (unsigned) mmap->type);

    }

    if ((mb_ptr->flags & MULTIBOOT_INFO_DRIVE_INFO) == MULTIBOOT_INFO_DRIVE_INFO) {
        printf("Drives: 0x%x, size: %d\n", mb_ptr->drives_addr, mb_ptr->drives_length);
    }

    if ((mb_ptr->flags & MULTIBOOT_INFO_CONFIG_TABLE) == MULTIBOOT_INFO_CONFIG_TABLE) {
        printf("Config: 0x%x\n", mb_ptr->config_table);
    }

    // Check
    if ((mb_ptr->flags & MULTIBOOT_INFO_BOOT_LOADER_NAME) == MULTIBOOT_INFO_BOOT_LOADER_NAME) {
        printf("Loaded by: %s\n", (char *)mb_ptr->boot_loader_name);
    }

    if ((mb_ptr->flags & MULTIBOOT_INFO_APM_TABLE) == MULTIBOOT_INFO_APM_TABLE) {
        printf("APM: 0x%x\n", mb_ptr->apm_table);
    }

    // Check
    if ((mb_ptr->flags & MULTIBOOT_INFO_VBE_INFO) == MULTIBOOT_INFO_VBE_INFO) {
        printf("VBE: 0x%x & 0x%x, mode: %d\n", mb_ptr->vbe_control_info, mb_ptr->vbe_mode_info, mb_ptr->vbe_mode);
    }

    // Check
    if ((mb_ptr->flags & MULTIBOOT_INFO_FRAMEBUFFER_INFO) == MULTIBOOT_INFO_FRAMEBUFFER_INFO) {
        printf("FB: 0x%x, pitch: %d, %dx%dx%d, type: %d\n",
                (uint32_t)mb_ptr->framebuffer_addr,
                mb_ptr->framebuffer_pitch,
                mb_ptr->framebuffer_width,
                mb_ptr->framebuffer_height,
                mb_ptr->framebuffer_bpp,
                mb_ptr->framebuffer_type);
    }
}

void kernel_main(multiboot_info_t *mb_ptr, uint32_t stack_ptr) {
    terminal_initialize();
    init_gdt();
    init_idt();
    init_isr();
    init_irq();
    init_timer();
    init_keyboard();

    printf("Kernel start: 0x%x, end: 0x%x\n", &__mboot, &__end);
    printf("Stack: 0x%x 0x%x\n", stack_ptr, &stack_ptr);
    debug_flags(mb_ptr);

    __asm__ __volatile__ ("sti");

    for(;;) {
        __asm__ __volatile__ ("hlt");
        unsigned char c = getchar();
        if (c > 0) {
            if (c == 27) {
                terminal_initialize();
            } else {
                putchar(c);
            }
        }
    }

}
