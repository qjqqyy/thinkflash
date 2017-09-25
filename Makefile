PROG	 = thinkflash

DESTDIR ?= /usr/local/sbin
CC	?= cc
CFLAGS	+= -std=c99 -Wall -Wextra -pedantic
INSTALL	?= install -s -m4550 -o root -g operator

$(PROG):

.PHONY: clean install

clean:
	rm -f $(PROG)

install: $(PROG)
	$(INSTALL) $(PROG) $(DESTDIR)

uninstall:
	rm -f $(DESTDIR)/$(PROG)
