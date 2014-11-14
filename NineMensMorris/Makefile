CFLAGS = -g -Wall -Werror -std=c99
OUT = -o bin/client
CC = gcc

all: client performConnection clean


client:	src/main.c
	$(CC) $(CFLAGS) $(OUT) src/main.c 

performConnection: src/connector/performConnection.c
	$(CC) $(CFLAGS) -o bin/performConnection src/connector/performConnection.c

pc: performConnection

pcwlog:
	$(CC) $(CFLAGS)  src/logger/logger.c src/connector/performConnection.c -o bin/performConnectionwLog

clean:
	rm -f src/*.o
	rm -f src/*.a
