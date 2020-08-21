SYSTEM_HEADER_PROJECTS=libc kernel
PROJECTS=libc kernel

HOST=i686-elf
HOSTARCH!=if echo $(HOST) | grep -Eq 'i[[:digit:]]86-'; then \
	  echo i386; \
	else \
	  echo "$(HOST)" | grep -Eo '^[[:alnum:]_]*'; \
	fi

MAKE?=make
AR=$(HOST)-ar
AS=$(HOST)-as
CC=$(HOST)-gcc

PREFIX=/usr
EXEC_PREFIX=$(PREFIX)
BOOTDIR=/boot
LIBDIR=$(EXEC_PREFIX)/lib
INCLUDEDIR=$(PREFIX)/include

CFLAGS=-O2 -g
CPPFLAGS=

# Configure the cross-compiler to use the desired system root.
SYSROOT=$(shell pwd)/sysroot
CC:=$(CC) --sysroot=$(SYSROOT)

## Work around that the -elf gcc targets doesn't have a system include directory
## because it was configured with --without-headers rather than --with-sysroot.
#if echo "$HOST" | grep -Eq -- '-elf($|-)'; then
#  export CC="$CC -isystem=$INCLUDEDIR"
CC:=$(CC) -isystem=$(INCLUDEDIR)
#fi

define GRUB
set timeout=1
menuentry "Genesis" {
	multiboot /boot/kernel.bin args
}
endef

export

.PHONY: all clean headers build iso reset docker

all: iso qemu

headers:
	mkdir -p $(SYSROOT)
	for PROJECT in $(SYSTEM_HEADER_PROJECTS); do \
	  DESTDIR="$(SYSROOT)" $(MAKE) -C $${PROJECT} install-headers; \
	done

build: headers
	for PROJECT in $(PROJECTS); do \
	  DESTDIR="$(SYSROOT)" $(MAKE) -C $${PROJECT} install; \
	done

iso: build
	mkdir -p isodir
	mkdir -p isodir/boot
	mkdir -p isodir/boot/grub
	cp sysroot/boot/kernel.bin isodir/boot/kernel.bin
	echo "$$GRUB" > isodir/boot/grub/grub.cfg
	grub-mkrescue -o genesis.iso isodir

qemu: genesis.iso
	qemu-system-$(HOSTARCH) -cdrom genesis.iso

clean:
	for PROJECT in $(PROJECTS); do \
	  $(MAKE) -C $${PROJECT} clean; \
	done
	-rm -rf sysroot
	-rm -rf isodir
	-rm -rf genesis.iso

reset:
	find . -type d -exec chmod 0755 {} \;
	find . -type f -exec chmod 0644 {} \;

docker:
	docker build -t genesis docker
