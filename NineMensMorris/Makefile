CFLAGS = -g -Wall -Werror -std=c99
OUT = -o bin/client
CC = gcc

all: src/main.c src/performConnection.c
	$(CC) $(CFLAGS) $(OUT) src/main.c

clean:
	rm -f *.o
	rm -f *.a
