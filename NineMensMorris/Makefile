CFLAGS = -g -Wall -Werror -std=c99
OUT = -o bin/client
CC = gcc

all: client clean

client:	src/main.c
	$(CC) $(CFLAGS) $(OUT) src/main.c

performConnection: src/performConnection.c clean
	$(CC) $(CFLAGS) -o bin/performConnection src/performConnection.c

clean:
	rm -f src/*.o
	rm -f src/*.a
