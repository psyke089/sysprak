#include "client_server.h"

int main(int argc, char const *argv[])
{

    struct sockaddr_in server_addr;
    struct sockaddr_in client_addr;

    int socket_server;
    int socket_client;

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
        //wait for reply
        printf("waiting for reply...\n");
        bzero(in_buf, MSGL);
        get_message(socket_client, in_buf);
        printf("client sayz: %s\n", in_buf);

        //get input
        printf("Message to client: ");
        bzero(out_buf, MSGL);
        fgets(out_buf, MSGL, stdin);
        if (strcmp(out_buf,"quit\n") == 0){
            break;
        }

        //send input
        printf("sending: %s\n", out_buf);
        send_message(socket_client, out_buf);
    }


    close(socket_client);
    close(socket_server);


    return 0;
}