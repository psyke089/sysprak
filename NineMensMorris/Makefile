CFLAGS = -g -Wall -Werror -std=c99 -D_XOPEN_SOURCE=600
MACFLAGS = -g -Wall -Werror -std=c99 -D_XOPEN_SOURCE=600 -D_GNU_SOURCE
EFLAGS = -g -Wall -D_XOPEN_SOURCE=600

OUT = -o bin/client
CC = gcc


all:    client clean

client: src/logger/logger.h\
				src/logger/logger.c\
				src/shm/shmManager.c\
				src/shm/shmManager.h\
				src/thinker/thinker.h\
				src/thinker/thinker.c\
				src/config.h\
				src/config.c\
				src/connector/connector.h\
				src/connector/performConnection.c\
				src/connector/parser.c\
				src/main.h\
				src/main.c

	$(CC) $(CFLAGS) $(OUT) \
		src/config.c\
		src/main.c\
		src/logger/logger.c\
		src/shm/shmManager.c\
		src/thinker/thinker.c\
		src/connector/performConnection.c\
		src/connector/parser.c

clean:
	rm -f src/*.o
	rm -f src/*.a
