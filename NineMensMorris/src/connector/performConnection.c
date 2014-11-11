#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <strings.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/ioctl.h>
#include <sys/select.h>
#include <errno.h>

#include "../main.h"

#define MSGL 4096


char words[32][64];

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
 * parseServerMsg parsed die Nachricht die vom Server kommt
 * sie benutzt das Leerzeichen als delimiter und speichert dir Teile als Array
 *
 * aktuelle maximale Wortlänge sind 64 Zeichen
 *
 * buf: die zu parsende Nachricht
 */

void parseServerMsg(char *buf){

  //char words[32][64];
  char word [64];
  char buf2[MSGL];
  int wordLength = 0;
  int wordCount = 0;

  for (int i = 0; i < 32; ++i)
  {
    bzero(words[i], 64);
  }

  sprintf(buf2, "%s", buf);

  while ( sscanf(buf, "%63[^ ]%n", word, &wordLength) == 1 ){

    sprintf(words[wordCount], "%s", word);
    //printf("parsedMsg Nr.%i\t::  \"%s\"\n", wordCount, word);
    ++wordCount;

    buf += wordLength;
    if ( *buf != ' ' ){
      break;
    }
    ++buf;

  }

  switch (*words[0]) {
    case '+' :
      printf("=> : " GREEN);
      break;
    case '-' :
      printf("=> : " RED);
      break;
    default: break;
  }

  printf("%s" RESET, buf2);

}

/**
 *  Liest von einem Socket.
 *
 *  sock: Socket filedescriptor
 *  buf:  Pointer auf buffer,
 *        in den die erhaltene Nachricht geschrieben werden soll
 */

int get_message(int sock, char* buf)
{

  int n;
  n = recv(sock, buf, MSGL, 0);
  //n = read(sock, buf, MSGL);
  if (n < 0)
  {
    if ((errno == EAGAIN) || (errno == EWOULDBLOCK)){
      printf("non-blocking operation returned EAGAIN or EWOULDBLOCK\n");
    }else{
      printf("recv returned unrecoverable error(errno=%d)\n", errno);
    }
    perror(RED "ERROR reading from socket" RESET);
    exit(1);
  }
  printf("\nreceived %i bytes:\n", n);
  return n;

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
  //n = write(sock, buf, MSGL);
  int len = strlen(buf);
  n = send(sock, buf, len, 0);
  if (n < 0)
  {
    perror(RED "ERROR writing to socket" RESET);
    exit(1);
  }

}

/**
 *  Setzt einen Socket in nonblocking mode.
 *
 *  sock: S
 */

int setNonblocking(int sock)
{
  int flags;
  if (-1 == (flags = fcntl(sock, F_GETFL, 0)))
    flags = 0;
  return fcntl(sock, F_SETFL, flags | O_NONBLOCK);
}


void errMessagesRoutine(){

  if (strcmp(words[1],"Socket") == 0 && strcmp(words[2],"timeout") == 0){
    printf(RED "You took too long to answer - you have ~3 second to send THINKING\n" RESET);
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

//  setNonblocking(le_socket);

  printf("\n");


  while(1)
  {

    bzero(in_buf, MSGL);
    get_message(le_socket, in_buf);
    parseServerMsg(in_buf);

    if(strcmp(words[0], "-") == 0) {

      printf(RED "negative response from server\n" RESET);
      break;

    }else if(strcmp(words[1], "MNM") == 0) {

      sprintf(out_buf, "VERSION %0.1f\n", CVERSION);
      send_message(le_socket, out_buf);

    }else if(strcmp(words[1], "Client") == 0) {

      sprintf(out_buf, "ID %s\n", argv[1]);
      send_message(le_socket, out_buf);

    }else if(strcmp(words[1], "PLAYING") == 0) {

      bzero(in_buf, MSGL);
      get_message(le_socket, in_buf);
      parseServerMsg(in_buf);

      sprintf(out_buf, "PLAYER\n");
      send_message(le_socket, out_buf);

    }else if(strcmp(words[1], "YOU") == 0) {

      printf(YELLOW "!YOU!\n" RESET);

    }else if(strcmp(words[1], "TOTAL") == 0) {

      printf(YELLOW "!TOTAL!\n" RESET);

      sprintf(out_buf, "THINKING\n");
      send_message(le_socket, out_buf);

    }else{

      printf(YELLOW "!DONE!\n" RESET);
      break;

    }

  }

  close(le_socket);

  return 0;
}
