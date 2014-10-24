CFLAGS = -g -Wall -std=c99

CC = gcc

all: netlib server client clean_nobin wurstbrot

netlib: netlib.c
	$(CC) $(CFLAGS) -c netlib.c
	ar -q netlib.a netlib.o

server: server.c netlib.a
	$(CC) $(CFLAGS) -o server server.c netlib.a

client: client.c netlib.a
	$(CC) $(CFLAGS) -o client client.c netlib.a

clean_nobin: 
	rm *.o rm *.a

clean:
	rm *.o *.a server client

wurstbrot:
	@toilet --gay GREAT SUCCESS! 