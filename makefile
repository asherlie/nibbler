CC=gcc
CFLAGS=  -Wall -Wextra -Wpedantic -Werror -O3 -lpthread -lcurl
#CFLAGS=  -Wall -Wextra -Wpedantic -Werror -g -lpthread -lcurl

all: nib

strhash.o: strhash.c strhash.h
dl.o: dl.c dl.h
tagger.o: tagger.c tagger.h dl.o strhash.o
#nib: nib.c strhash.o
nib: nib.c strhash.o dl.o tagger.o

.PHONY:
clean:
	rm -f nib *.o
