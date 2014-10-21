#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h> 
#include <netinet/in.h>

#define PORT 4711
#define MSGL 256
#define QUITCMD "quit"

int create_socket()
{	
	int ret;
	ret = socket(AF_INET, SOCK_STREAM, 0);
	if (ret < 0)
	{
		printf("Could not create socket");
		exit(1);
	}
	return ret;
}

void bind_socket(int sock, struct sockaddr_in serv_addr)
{	
	int n;
	n = bind(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
	if (n < 0)
    {
        printf("ERROR on binding");
        exit(1);
    }
}

void get_message(int sock, char* msg)
{
	int n;
	n = read(sock, msg, MSGL-1);
	if (n < 0)
    {
        printf("ERROR reading from socket");
        exit(1);
    }
}

void send_message(int sock, char* msg)
{	
	int n;
	n = write(sock, msg, MSGL-1);
	if (n < 0)
    {
        printf("ERROR writing to socket");
        exit(1);
    }
	
}

int accept_connection(int sock, struct sockaddr_in cli_addr)
{
	int ret;
	int cli_addr_len = sizeof(cli_addr);
	ret = accept(sock, (struct sockaddr *)&cli_addr, &cli_addr_len);
    if (ret < 0) 
    {
        printf("ERROR on accept");
        exit(1);
    }
    return ret;
}

int main(int argc, char const *argv[])
{

	struct sockaddr_in server_addr;
	struct sockaddr_in client_addr;

	int client_addr_len = sizeof(client_addr);
	int socket_server;
	int socket_client;
	int n;

	char in_buf[MSGL];
	char out_buf[MSGL];

	bzero(in_buf, MSGL);
	bzero(out_buf, MSGL);


	socket_server = create_socket();


	bzero((char *) &server_addr, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = INADDR_ANY;
	server_addr.sin_port = htons(PORT);


	bind_socket(socket_server, server_addr);


    listen(socket_server, 5);


    socket_client = accept_connection(socket_server, client_addr);




    while(1)
    {
    	get_message(socket_client, in_buf);

	    printf("client sayz: %s\n", in_buf);

	    scanf("message to client: %s", out_buf);

	    send_message(socket_client, out_buf);
    }


	return 0;
}