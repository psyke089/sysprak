CFLAGS = -g -Wall -Werror -std=c99 -D_XOPEN_SOURCE=600
MACFLAGS = -g -Wall -Werror -std=c99 -D_XOPEN_SOURCE=600 -D_GNU_SOURCE
EFLAGS = -g -Wall -D_XOPEN_SOURCE=600

OUT = -o bin/client
CC = gcc


all:    client clean
alex_i: client_alex_i clean
alex_h: client_alex_h clean
tim:    client_tim    clean
phil:   client_phil   clean

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

client_alex_h: src/logger/logger.h\
				src/logger/logger.c\
				src/shm/shmManager.c\
				src/shm/shmManager.h\
				src/thinker/thinker_alex_h.h\
				src/thinker/thinker_alex_h.c\
				src/config.h\
				src/config.c\
				src/connector/connector.h\
				src/connector/performConnection.c\
				src/connector/parser.c\
				src/main.h\
				src/main.c

	$(CC) $(CFLAGS) -o bin/client_alex_h \
		src/config.c\
		src/main.c\
		src/logger/logger.c\
		src/shm/shmManager.c\
		src/thinker/thinker_alex_h.c\
		src/connector/performConnection.c\
		src/connector/parser.c

client_tim: src/logger/logger.h\
				src/logger/logger.c\
				src/shm/shmManager.c\
				src/shm/shmManager.h\
				src/thinker/thinker_tim.h\
				src/thinker/thinker_tim.c\
				src/config.h\
				src/config.c\
				src/connector/connector.h\
				src/connector/performConnection.c\
				src/connector/parser.c\
				src/main.h\
				src/main.c

	$(CC) $(CFLAGS) -o bin/client_tim \
		src/config.c\
		src/main.c\
		src/logger/logger.c\
		src/shm/shmManager.c\
		src/thinker/thinker_alex_i.c\
		src/connector/performConnection.c\
		src/connector/parser.c



client_alex_i: src/logger/logger.h\
				src/logger/logger.c\
				src/shm/shmManager.c\
				src/shm/shmManager.h\
				src/thinker/thinker_alex_i.h\
				src/thinker/thinker_alex_i.c\
				src/config.h\
				src/config.c\
				src/connector/connector.h\
				src/connector/performConnection.c\
				src/connector/parser.c\
				src/main.h\
				src/main.c

	$(CC) $(CFLAGS) -o bin/client_alex_i \
		src/config.c\
		src/main.c\
		src/logger/logger.c\
		src/shm/shmManager.c\
		src/thinker/thinker_alex_i.c\
		src/connector/performConnection.c\
		src/connector/parser.c

client_phil: src/logger/logger.h\
				src/logger/logger.c\
				src/shm/shmManager.c\
				src/shm/shmManager.h\
				src/thinker/thinker_alex_i.h\
				src/thinker/thinker_alex_i.c\
				src/config.h\
				src/config.c\
				src/connector/connector.h\
				src/connector/performConnection.c\
				src/connector/parser.c\
				src/main.h\
				src/main.c

	$(CC) $(CFLAGS) -o bin/client_phil \
		src/config.c\
		src/main.c\
		src/logger/logger.c\
		src/shm/shmManager.c\
		src/thinker/thinker_alex_i.c\
		src/connector/performConnection.c\
		src/connector/parser.c
clean:
	rm -f src/*.o
	rm -f src/*.a
