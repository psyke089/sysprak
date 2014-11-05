#include "client_server.h"

int create_socket()
{   
    int fd;
    fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd < 0)
    {
        printf("Could not create socket");
        exit(1);
    }
    printf("socket created...\n");
    return fd;
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
    printf("socket bound...\n");
}

void get_message(int sock, char* msg)
{
    int n;
    n = read(sock, msg, MSGL);
    if (n < 0)
    {
        printf("ERROR reading from socket");
        exit(1);
    }
}

void send_message(int sock, char* msg)
{   
    int n;
    n = write(sock, msg, MSGL);
    if (n < 0)
    {
        printf("ERROR writing to socket");
        exit(1);
    }
    
}

int accept_connection(int sock, struct sockaddr_in cli_addr)
{
    int fd;
    socklen_t cli_addr_len = sizeof(cli_addr);
    fd = accept(sock, (struct sockaddr *)&cli_addr, &cli_addr_len);
    if (fd < 0) 
    {
        printf("ERROR on accept");
        exit(1);
    }
    printf("connection accepted...\n");
    return fd;
}

void connect_to_socket(int sock, struct sockaddr_in dest)
{   
    int n;
    n = connect(sock , (struct sockaddr *)&dest , sizeof(dest));
    if (n < 0)
    {
        printf("connect failed. Error");
        exit(1);
    }
    printf("connected to socket...\n");
}