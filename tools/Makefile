
CC=gcc

TOOLS=\
make_initrd \

.PHONY: all install

all: $(TOOLS)

install: all
	mkdir -p $(DESTDIR)/bin
	for TOOL in $(TOOLS); do \
	  cp $${TOOL} $(DESTDIR)/bin; \
	done

clean:
	-rm -f $(TOOLS)
