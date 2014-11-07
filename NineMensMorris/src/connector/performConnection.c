#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <strings.h>
#include <arpa/inet.h>
#include <netdb.h>

#include "../main.h"

#define MSGL 256


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
  int wordLength = 0;
  int wordCount = 0;

  for (int i = 0; i < 32; ++i)
  {
    bzero(words[i], 64);
  }
  

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
      printf("=> : " ANSI_COLOR_GREEN);
      break;
    case '-' :
      printf("=> : " ANSI_COLOR_RED);
      break;
    default: break;
  }

  for (int i = 0; i < 32; ++i)
  {
    if (strcmp(words[i], "") == 0) {
      //printf(ANSI_COLOR_RESET "Words: %i\n", i);
      break;
    }
    printf("%s ", words[i]);
  }
  printf("\n" ANSI_COLOR_RESET);

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
    printf("<= : %s\n", buf);
    n = write(sock, buf, MSGL);
    if (n < 0)
    {
        printf("ERROR writing to socket");
        exit(1);
    }

}



int main(int argc, char const *argv[])
{
  
  //char id[] = "Y.Z36w4VXdc#";

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
    
    bzero(in_buf, MSGL);
    get_message(le_socket, in_buf);
    parseServerMsg(in_buf);

    if(strcmp(words[1], "MNM") == 0) {

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
      
      bzero(in_buf, MSGL);
      get_message(le_socket, in_buf);
      parseServerMsg(in_buf);

    }

  }

/*
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
*/

  close(le_socket);

  return 0;
}