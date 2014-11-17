#include "connector.h"

char words[32][64];
char id[11];

/**
 * parseSingleMessage parsed die Nachricht die vom Server kommt
 * sie benutzt das Leerzeichen als delimiter und speichert dir Teile als Array
 *
 * aktuelle maximale Wortlänge sind 64 Zeichen
 *
 * buf: die zu parsende Nachricht
 */

void parseSingleMessage(char *buf)
{

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

  printf("%s\n" RESET, buf2);

}

void parseMessages(int sock)
{

  int cont = 1;

  char in_buf[MSGL];
  char out_buf[MSGL];

  bzero(in_buf, MSGL);
  bzero(out_buf, MSGL);

  
  printf("\n");

  while(cont)
  {

    bzero(in_buf, MSGL);
    get_message(sock, in_buf);
    parseSingleMessage(in_buf);

    switch (*words[0]) {
      case '+' :
        
        if(strcmp(words[1], "MNM") == 0) {

          sprintf(out_buf, "VERSION %0.1f\n", CVERSION);
          send_message(sock, out_buf);

        }else if(strcmp(words[1], "Client") == 0) {

          sprintf(out_buf, "ID %s\n", id);
          send_message(sock, out_buf);

        }else if(strcmp(words[1], "PLAYING") == 0) {

          bzero(in_buf, MSGL);
          get_message(sock, in_buf);
          parseSingleMessage(in_buf);

          sprintf(out_buf, "PLAYER\n");
          send_message(sock, out_buf);

        }else if(strcmp(words[1], "YOU") == 0) {

          printf(YELLOW "!YOU!\n" RESET);
          //logPrnt('y','s',"!YOU!\n");

        }else if(strcmp(words[1], "TOTAL") == 0) {

          printf(YELLOW "!TOTAL!\n" RESET);
          //logPrnt('y','s',"!TOTAL!\n");

          sprintf(out_buf, "THINKING\n");
          send_message(sock, out_buf);

        }else if(strcmp(words[1], "OKTHINK\n") == 0){
          printf(YELLOW "...thinking...\n" RESET);
          //logPrnt('y','s',"...thinking...\n");

          // here comes the forking action

        }else if(strcmp(words[1], "WAIT\n") == 0){
          
          sprintf(out_buf, "OKWAIT\n");
          send_message(sock, out_buf);

        }else{

          printf(YELLOW "...out of options.\n" RESET);
          //logPrnt('y','s',"...out of options.\n");
          break;

        }

        break;

      case '-' :
        if(strcmp(words[2], "timeout") == 0){
          printf(YELLOW "...Socket timeout.\n" RESET);
        }
        cont = 0;
        break;

      default: 
        printf(YELLOW "Unexpected message from server.\n" RESET);
        cont = 0;
        break;
    }

    

  }
}