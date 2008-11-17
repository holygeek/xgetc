#PREFIX=/usr/local
PREFIX=$$HOME

FILES = xgetc.c xgetc.1 LICENSE makefile

LIBS=`pkg-config --libs x11`

all: xgetc

xgetc: xgetc.c
	gcc $(LIBS) xgetc.c -o xgetc 

install: xgetc
	install xgetc $(PREFIX)/bin
	install -m 644 xgetc.1 $(PREFIX)/man/man1

clean:
	rm -f xgetc xgetc.tar.gz

dist: $(FILES)
	@mkdir -p ./tmp/xgetc
	@cp $(FILES) ./tmp/xgetc
	@cd ./tmp && tar czf xgetc.tar.gz xgetc && mv xgetc.tar.gz ..
	@rm -rf ./tmp
