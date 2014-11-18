CFLAGS = -g -Wall -Werror -std=c99 -D_XOPEN_SOURCE=600
OUT = -o bin/client
CC = gcc

all: client clean

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

client: src/shm/shmManager.c\
				    src/shm/shmManager.h\
						src/config.h\
						src/config.c\
						src/main.h\
						src/main.c
						
	$(CC) $(CFLAGS) $(OUT)\
		src/shm/shmManager.c\
		src/config.c\
		src/main.c

clean:
	rm -f src/*.o
	rm -f src/*.a
