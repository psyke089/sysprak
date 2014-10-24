#include "client_server.h"

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
    dest.sin_addr.s_addr = inet_addr( argv[1] );
    dest.sin_family = AF_INET;
    dest.sin_port = htons( PORT );


    connect_to_socket(socket_client, dest);




    while(1)
    {   
        //get input
        printf("Message to server: ");
        bzero(out_buf, MSGL);
        fgets(out_buf, MSGL, stdin);
        if (strcmp(out_buf,"quit\n") == 0){
            break;
        }
        
        //send input
        printf("sending: %s\n", out_buf);
        send_message(socket_client, out_buf);
        
        //wait for reply
        printf("waiting for reply...\n");
        bzero(in_buf, MSGL);
        get_message(socket_client, in_buf);
        printf("server sayz: %s\n", in_buf); 
    }
    
     
    close(socket_client);


    return 0;

}