#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <strings.h>
#include <arpa/inet.h>
#include <netdb.h>

#include "../main.h"

#define MSGL 256

/**
 *  Löst HOSTNAME in eine Ip auf und
 *  erstellt und initialisiert ein sockaddr_in struct daraus. 
 *
 *  returns: struct sockaddr_in
 */

struct sockaddr_in init_server_addr()
{
  
  struct hostent *hp = gethostbyname(HOSTNAME);
  struct sockaddr_in ret;

  if (hp == NULL) {
    printf("gethostbyname() failed\n");
    exit(1);
  }

  /* server_addr initilaisieren */
  bzero((char *) &ret, sizeof(ret));
  
  /* copy the network address to sockaddr_in structure */
  memcpy(&ret.sin_addr, hp->h_addr_list[0], hp->h_length);

  ret.sin_family = AF_INET;
  ret.sin_port = htons( PORTNUMBER );

  printf("%s = ", hp->h_name);

  return ret;
}

/**
 *  Erstellt einen Socket.
 *
 *  returns: Socket filedescriptor
 */

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

/**
 *  Connectet zu einem Socket.
 *
 *  sock: Socket filedescriptor
 *  dest: sockaddr_in
 */

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

/**
 *  Liest von einem Socket.
 *
 *  sock: Socket filedescriptor
 *  msg: Die erhaltene Nachricht.
 */

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

/**
 *  Schreibt in einen Socket.
 *
 *  sock: Socket filedescriptor
 *  msg: Die zu schreibende Nachricht.
 */

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

int main(int argc, char const *argv[])
{
  

  int le_socket;
  struct sockaddr_in server_addr;

  char in_buf[MSGL];
  char out_buf[MSGL];

  bzero(in_buf, MSGL);
  bzero(out_buf, MSGL);


  
  server_addr = init_server_addr();

  printf( "%s ", inet_ntoa( *( struct in_addr*)( &server_addr.sin_addr )));
  printf("\n");

  le_socket = create_socket();

  connect_to_socket(le_socket, server_addr);

  while(1)
  {
    
    bzero(in_buf, MSGL);
    get_message(le_socket, in_buf);
    printf("server sayz: %s\n", in_buf);

    //get input
    printf("> ");
    bzero(out_buf, MSGL);
    fgets(out_buf, MSGL, stdin);
    if (strcmp(out_buf,"quit\n") == 0){
        break;
    }

    //send inputD
    send_message(le_socket, out_buf);

    //wait for reply
    printf("-------------------------\n");

  }

  close(le_socket);

  return 0;
}