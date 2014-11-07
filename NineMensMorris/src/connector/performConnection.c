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

  // server_addr initilaisieren
  bzero((char *) &ret, sizeof(ret));
  
  //Server-IP in sockaddr_in struct kopieren
  memcpy(&ret.sin_addr, hp->h_addr_list[0], hp->h_length);

  ret.sin_family = AF_INET;
  ret.sin_port = htons( PORTNUMBER );

  printf("%s = ", hp->h_name);
  printf( "%s\n", inet_ntoa( *( struct in_addr*)( &ret.sin_addr )));

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
 *  buf:  Pointer auf buffer, 
 *        in den die erhaltene Nachricht geschrieben werden soll
 */

void get_message(int sock, char* buf)
{
    int n;
    n = read(sock, buf, MSGL);
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
 *  buf:  Pointer auf buffer mit zu schreibender Nachricht
 */

void send_message(int sock, char* buf)
{
    int n;
    n = write(sock, buf, MSGL);
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


  printf("\n");

  server_addr = init_server_addr();

  le_socket = create_socket();

  connect_to_socket(le_socket, server_addr);

  printf("\n");
  while(1)
  {
    
    //warten auf Nachricht vom Server
    bzero(in_buf, MSGL);
    get_message(le_socket, in_buf);
    printf("%s\n", in_buf);

    //get input
    printf("> ");
    bzero(out_buf, MSGL);
    fgets(out_buf, MSGL, stdin);
    printf("\n");
    if (strcmp(out_buf,"quit\n") == 0){
        break;
    }

    //send input
    send_message(le_socket, out_buf);

  }

  close(le_socket);

  return 0;
}