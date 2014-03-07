PREFIX=/usr/local
#PREFIX=$$HOME

FILES = xgetc.c xgetc.1 LICENSE makefile README

LIBS=`pkg-config --libs x11`

all: xgetc

xgetc: xgetc.c
	gcc xgetc.c -o xgetc $(LIBS)

install: xgetc
	install xgetc $(PREFIX)/bin
	install -m 644 xgetc.1 $(PREFIX)/man/man1

clean:
	rm -f xgetc xgetc.tar.gz

VERSION=0.1
dist: $(FILES)
	@mkdir -p ./tmp/xgetc-$(VERSION)
	@cp $(FILES) ./tmp/xgetc-$(VERSION)
	@cd ./tmp && tar czf xgetc-$(VERSION).tar.gz xgetc-$(VERSION) && mv xgetc-$(VERSION).tar.gz ..
	@rm -rf ./tmp
