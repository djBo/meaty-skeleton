#ifndef KERNEL_PAGING_H
#define KERNEL_PAGING_H

#include <kernel/multiboot.h>

void init_paging(multiboot_info_t *mb_ptr);

#endif /* KERNEL_PAGING_H */
