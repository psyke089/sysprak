CFLAGS = -g -Wall -Werror -std=c99 -D_XOPEN_SOURCE=600 
MACFAGS = -g -Wall -Werror -std=c99 -D_XOPEN_SOURCE=600 -D_GNU_SOURCE
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


# funktioniert nicht mehr, @todo mit parser etc compilen
pcwlog: src/logger/logger.c\
				src/connector/performConnection.c

	$(CC) $(CFLAGS) -o bin/performConnectionwLog \
		src/logger/logger.c \
		src/connector/performConnection.c

loggertest:
	$(CC) $(CFLAGS)  src/logger/logger.c  src/logger/logger_test_main.c -o bin/loggerTest

client: src/logger/logger.h\
				src/logger/logger.c\
				src/shm/shmManager.c\
				src/shm/shmManager.h\
				src/thinker/thinker.c\
				src/thinker/thinker.h\
				src/config.h\
				src/config.c\
				src/main.h\
				src/main.c
						
	$(CC) $(CFLAGS) $(OUT) \
		src/config.c\
		src/main.c\
		src/logger/logger.c\
		src/shm/shmManager.c\
		src/thinker/thinker.c

mac: maccomp clean
maccomp:  src/logger/logger.h\
				src/logger/logger.c\
				src/shm/shmManager.c\
				src/shm/shmManager.h\
				src/thinker/thinker.c\
				src/thinker/thinker.h\
				src/config.h\
				src/config.c\
				src/main.h\
				src/main.c
						
	$(CC) $(MACFLAGS) $(OUT)\
		src/config.c\
		src/main.c\
		src/logger/logger.c\
		src/shm/shmManager.c\
		src/thinker/thinker.c
		
		

clean:
	rm -f src/*.o
	rm -f src/*.a
