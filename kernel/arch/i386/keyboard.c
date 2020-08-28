#include <kernel/irq.h>
#include <kernel/isr.h>
#include <kernel/sysio.h>

static unsigned char kbdus[256] = {
       0,   27,  '1',  '2',  '3',  '4',  '5',  '6',
     '7',  '8',  '9',  '0',  '-',  '=', '\b', '\t',
     'q',  'w',  'e',  'r',  't',  'y',  'u',  'i',
     'o',  'p',  '[',  ']', '\n',    0,  'a',  's',
     'd',  'f',  'g',  'h',  'j',  'k',  'l',  ';',
    '\'',  '`',    0, '\\',  'z',  'x',  'c',  'v',
     'b',  'n',  'm',  ',',  '.',  '/',    0,  '*',
       0,  ' ',    0,    0,    0,    0,    0,    0,
       0,    0,    0,    0,    0,    0,    0,    0,
       0,    0,  '-',    0,    0,    0,  '+',    0,
       0,    0,    0,    0,    0,    0,    0,    0,
       0,

             0,    0,    0,    0,    0,    0,    0,
       0,    0,    0,    0,    0,    0,    0,    0,
       0,    0,    0,    0,    0,    0,    0,    0,
       0,    0,    0,    0,    0,    0,    0,    0,
       0,    0,    0,    0,    0,    0,    0,    0,

       0,   27,  '!',  '@',  '#',  '$',  '%',  '^',
     '&',  '*',  '(',  ')',  '_',  '+', '\b', '\t',
     'Q',  'W',  'E',  'R',  'T',  'Y',  'U',  'I',
     'O',  'P',  '{',  '}', '\r',    0,  'A',  'S',
     'D',  'F',  'G',  'H',  'J',  'K',  'L',  ':',
     '"',  '~',    0,  '|',  'Z',  'X',  'C',  'V',
     'B',  'N',  'M',  '<',  '>',  '?',    0,  '*',
       0,  ' ',    0,    0,    0,    0,    0,    0,
       0,    0,    0,    0,    0,    0,    0,    0,
       0,    0,  '-',    0,    0,    0,  '+',    0,
       0,    0,    0,    0,    0,    0,    0,    0,
       0,

             0,    0,    0,    0,    0,    0,    0,
       0,    0,    0,    0,    0,    0,    0,    0,
       0,    0,    0,    0,    0,    0,    0,    0,
       0,    0,    0,    0,    0,    0,    0,    0,
       0,    0,    0,    0,    0,    0,    0,    0

};

static unsigned char buffer[256] = { 0 };
static unsigned int index = 0;
static unsigned char shift = 0;

static void keyboard_putc(unsigned char c) {
	if (c == 0 || index == 256)
		return;
	buffer[index] = c;
	index++;
}

unsigned char keyboard_getch() {
	if (index == 0)
		return 0;
	__asm__ __volatile__ ("cli");
	// We should not be interrupted!
	unsigned char res = buffer[--index];
	__asm__ __volatile__ ("sti");
	return res;
}

void keyboard_handler(__attribute__((unused)) registers_t *regs) {
	unsigned char scancode;
	scancode = inportb(0x60);

	if (scancode & 0x80) {
		// on_key_up
		switch (scancode - 0x80) {
		case 42: // LSHIFT
		case 54: // RSHIFT
			shift = 0;
		}
	} else {
		// on_key_down
		switch (scancode) {
		case 42: // LSHIFT
		case 54: // RSHIFT
			shift = 0x80;
			break;
		default:
			keyboard_putc(kbdus[scancode + shift]);
		}
	}
}

void init_keyboard() {
	install_interrupt_handler(1, keyboard_handler);
}
