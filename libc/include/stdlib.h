#ifndef _STDLIB_H
#define _STDLIB_H 1

#include <sys/cdefs.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _div_t {
	int quot;
	int rem;
} div_t;

__attribute__((__noreturn__))
void abort(void);

int abs (int i);
div_t div (int numer, int denom);
int div2(int numer, int denom);
void itoa(int input, char *buffer, int radix);

int rand();
void srand(unsigned int);

#ifdef __cplusplus
}
#endif

#endif
