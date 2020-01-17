CC=gcc
CFLAGS=  -Wall -Wextra -Wpedantic -Werror -O3 -lpthread -lcurl

all: nib

strhash.o: strhash.c strhash.h
nib: nib.c strhash.o

.PHONY:
clean:
	rm -f nib *.o
