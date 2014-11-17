CFLAGS = -g -Wall -Werror -std=c99
OUT = -o bin/client
CC = gcc

all: client clean


client: src/main.h src/main.c
	$(CC) $(CFLAGS) $(OUT) src/main.c 


connector: 	src/main.h\
						src/connector/connector.h\
						src/connector/connector.c\
						src/connector/performConnection.c\
						src/connector/parser.c

	$(CC) $(CFLAGS) -o bin/connector \
		src/connector/connector.c \
		src/connector/performConnection.c \
		src/connector/parser.c


pcwlog: src/logger/logger.c\
				src/connector/performConnection.c

	$(CC) $(CFLAGS) -o bin/performConnectionwLog \ 
		src/logger/logger.c \
		src/connector/performConnection.c


clean:
	rm -f src/*.o
	rm -f src/*.a
