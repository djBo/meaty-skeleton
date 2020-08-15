#ifndef _KERNEL_SYSIO_H
#define _KERNEL_SYSIO_H 1

#define EOF (-1)

unsigned char inportb (unsigned short port);
unsigned short inportw (unsigned short port);
void outportb(unsigned short port, unsigned char value);

#endif
