#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h> 
#include <netinet/in.h>

//#define IP "192.168.178.22"
#define IP "127.0.0.1"
#define PORT 4711
#define MSGL 256

int create_socket()
{   
    int fd;
    fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd < 0)
    {
        printf("Could not create socket");
        exit(1);
    }
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
}

void get_message(int sock, char* msg)
{
    int n;
    n = read(sock, msg, sizeof(msg));
    if (n < 0)
    {
        printf("ERROR reading from socket");
        exit(1);
    }
}

void send_message(int sock, char* msg)
{   
    int n;
    n = write(sock, msg, strlen(msg));
    if (n < 0)
    {
        printf("ERROR writing to socket");
        exit(1);
    }
    
}

int main (int argc, char* const argv[])
{
	int socket_client;
	struct sockaddr_in dest;

    char in_buf[256];
    char out_buf[256];

    bzero(in_buf, MSGL);
    bzero(out_buf, MSGL);



	socket_client = create_socket();

     
    bzero((char *) &dest, sizeof(dest));
    dest.sin_addr.s_addr = inet_addr( IP );
    dest.sin_family = AF_INET;
    dest.sin_port = htons( PORT );


    connect_to_socket(socket_client, dest);




    while(1)
    {   
        fgets(out_buf, MSGL, stdin);
        if (strcmp(in_buf,"quit") == 0){
            exit(0);
        }

        printf("sending: %s\n", out_buf);
        
        send_message(socket_client, out_buf);
        

        get_message(socket_client, in_buf);
        printf("server sayz: %s\n", in_buf); 
    }
    
     
    close(socket_client);


    return 0;

}
