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
	rm -f *.o
	rm -f *.a

clean:
	rm -f *.o
	rm -f *.a
	rm -f server
	rm -f client

wurstbrot:
	@toilet --gay GREAT
	@toilet --gay SUCCESS! 