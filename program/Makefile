HOST?=DEFAULT_HOST
HOSTARCH?=DEFAULT_ARCH

ASFLAGS=-f bin

DESTDIR?=
PREFIX?=/usr/local
EXEC_PREFIX?=$(PREFIX)
BOOTDIR?=$(EXEC_PREFIX)/boot

.PHONY: all clean install install-program

all: program

program:
	nasm $(ASFLAGS) program.s -o program
	$(HOST)-objdump -b binary -m $(HOSTARCH) --disassembler-options intel -DS program

clean:
	rm -f program
	rm -f *.o
	rm -f *.d

install: install-program

install-program: program
	mkdir -p $(DESTDIR)$(BOOTDIR)
	cp program $(DESTDIR)$(BOOTDIR)
