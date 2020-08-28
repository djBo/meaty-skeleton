#ifndef _KERNEL_H
#define _KERNEL_H

/* Check if the bit BIT in FLAGS is set. */
//#define CHECK_FLAG(flags,bit)   ((flags) & (1 << (bit)))
#define CHECK_FLAG(flags, flag)   (flags & flag) == flag

#endif /* _KERNEL_H */
