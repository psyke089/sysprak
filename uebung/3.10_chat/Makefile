CFLAGS = -g -Wall -std=c99

CC = gcc

all: server client

server: server.c 
	$(CC) $(CFLAGS) -o server server.c

client: client.c
	$(CC) $(CFLAGS) -o client client.c

clean:
	rm *.o server client