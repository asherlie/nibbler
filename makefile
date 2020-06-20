CC=gcc
#CFLAGS=  -Wall -Wextra -Wpedantic -Werror -O3 -lpthread -lcurl
#CFLAGS= -D_GNU_SOURCE -Wall -Wextra -Wpedantic -Werror -g -lpthread -lcurl
CFLAGS= -D_GNU_SOURCE -Wall -Wextra -Wpedantic -Werror -O3 -lpthread -lcurl

all: nib

strhash.o: strhash.c strhash.h
dl.o: dl.c dl.h
tagger.o: tagger.c tagger.h dl.o strhash.o
#nib: nib.c strhash.o
nib: nib.c strhash.o dl.o tagger.o

.PHONY:
db: nib.c strhash.o dl.o tagger.o
	gcc nib.c strhash.o dl.o tagger.o -DDEBUG -o nib $(CFLAGS)

.PHONY:
clean:
	rm -f nib *.o
