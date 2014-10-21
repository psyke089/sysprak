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
    int ret;
    ret = socket(AF_INET, SOCK_STREAM, 0);
    if (ret < 0)
    {
        printf("Could not create socket");
        exit(1);
    }
    printf("%i\n", ret);
    return ret;
}

void connect_to_socket(int sock, struct sockaddr_in dest)
{
    if (connect(sock , (struct sockaddr *)&dest , sizeof(dest)) < 0)
    {
        printf("connect failed. Error");
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

int main (int argc, char* const argv[])
{
	int socket_client;
	struct sockaddr_in dest;

    char in_buf[MSGL];
    char out_buf[MSGL];

    bzero(in_buf, MSGL);
    bzero(out_buf, MSGL);


	socket_client = create_socket();

     
    bzero((char *) &dest, sizeof(dest));
    dest.sin_addr.s_addr = inet_addr( IP );
    dest.sin_family = AF_INET;
    dest.sin_port = htons( PORT );


    connect_to_socket(socket_client, dest);


    send_message(socket_client, msgbuf);






    /* output response
    char buf[10000];
    while (recv(socket_client,buf,sizeof(buf)-1,0)) {
        printf("%s",buf);
    }
    */
     
    //close(socket_client);


    return 0;

}
