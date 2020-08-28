void outportb(unsigned short port, unsigned char value) {
	__asm__ __volatile__ ("outb %1, %0" : : "dN" (port), "a" (value));
}
