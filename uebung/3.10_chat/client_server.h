#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h> 
#include <netinet/in.h>
#include <unistd.h>
#include <strings.h>
#include <arpa/inet.h>

#define PORT 4711
#define MSGL 256

int create_socket();

void bind_socket(int sock, struct sockaddr_in serv_addr);

void get_message(int sock, char* msg);

void send_message(int sock, char* msg);

int accept_connection(int sock, struct sockaddr_in cli_addr);

void connect_to_socket(int sock, struct sockaddr_in dest);