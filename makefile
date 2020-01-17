CC=gcc
CFLAGS=  -Wall -Wextra -Wpedantic -Werror -O3 -lpthread -lcurl

all: nib

strhash.o: strhash.c strhash.h
tagger.o: tagger.c tagger.h strhash.o
#nib: nib.c strhash.o
dl.o: dl.c dl.h
nib: nib.c dl.o

.PHONY:
clean:
	rm -f nib *.o
