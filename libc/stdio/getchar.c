#include <stdio.h>

#if defined(__is_libk)
#include <kernel/keyboard.h>
#endif

int getchar() {
#if defined(__is_libk)
	return keyboard_getch();
#else
	// TODO: Implement stdio and the write system call.
#endif
	return 0;
}
