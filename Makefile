LDFLAGS=-L/usr/local/lib -lm
CFLAGS=-ggdb -Wall
CC=gcc
OBJ=interger.o book.o record.o sheet.o handler.o cell.o sstlist.o format.o formatting.o
HDR=biff.h interger.h record.h book.h sstlist.h format.h

all: xlreader

tester: test.o $(OBJ) $(HDR)
	$(CC) $(LDFLAGS) test.o $(OBJ) -o $@

xlreader: main.o $(OBJ) $(HDR)
	$(CC) $(LDFLAGS) main.o $(OBJ) -o $@

clean:
	rm -f *.o xlreader

dist-clean:
	rm -f *.o xlreader core *.core tester

install:
	install -m 755 xlreader /usr/local/bin

test: xlreader
	./xlreader --sql xlfiles/*

.SUFFIXES: .o .lo .mo $(HDR)
.c.o: $(HDR)
	$(CC) -c $(CFLAGS) $<
