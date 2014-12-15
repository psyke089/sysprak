#include "../main.h"
#include "../config.h"
#include "../shm/shmManager.h"
#include "connector.h"

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
    perror(RED "gethostbyname() failed\n" RESET);
    exit(1);
  }

  // server_addr initilaisieren
  memset ((char *) &ret, 0, sizeof(ret));

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
    perror(RED "Could not create socket" RESET);
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
    perror(RED "connect failed. Error" RESET);
    exit(1);
  }
  printf("connected to socket...\n");
}

/**
 *  Liest eine Zeile (bis '\n') von einem Socket.
 *
 *  sock: Socket filedescriptor
 *  buf:  Pointer auf buffer,
 *        in den die erhaltene Nachricht geschrieben werden soll
 */

void get_message(int sock, char* buf)
{

  char msg[MSGL];
  size_t bytes_received = 0;
  size_t n;

  memset(buf, 0, MSGL);
  int notbremse = 128;

  do{

    memset(msg, 0, MSGL);
    n = recv(sock, msg, MSGL, 0);

    if (n < 0)
    {
      perror(RED "ERROR reading from socket" RESET);
      //logPrnt('r','q',"ERROR reading from socket");
      exit(1);
    }

    bytes_received += n;
    strcat(buf,msg);

    //printf(CYAN "|" RESET "%s", msg);

    notbremse--;

  }while( (msg[n-1] != '\n') && notbremse > 0);

  printf("\nreceived %zu bytes:\n", bytes_received);

  switch(buf[0]){
    case '+':
      printf(GREEN);
      break;
    case '-':
      printf(RED);
      break;
    default:
      break;
  }

  printf("%s\n" RESET, buf);
  ///@todo in logfile integrieren @tim

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
  printf("<= : %s\n", buf);
  int len = strlen(buf);
  n = send(sock, buf, len, 0);
  if (n < 0)
  {
    perror(RED "ERROR writing to socket" RESET);
    // logPrnt('r','q',"ERROR writing to socket");
    exit(1);
  }

}

/**
 * Stellt die Verbindung her und startet den parser.
 */

int performConnection()
{

  int sock;
  struct sockaddr_in server_addr;

  printf("\n");

  server_addr = init_server_addr();
  sock = create_socket();
  connect_to_socket(sock, server_addr);

  return sock;
}
