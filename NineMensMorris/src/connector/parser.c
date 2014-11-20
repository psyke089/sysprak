#include "connector.h"

char id[11];
char msg_queue[32][128];

/**
 * Teilt eine Nachricht vom Server in einzelne Zeilen
 * und schreibt sie in die msg_queue.
 *
 * buf: die zu parsende Nachricht
 */

void processMessage(char *buf)
{
  
  int lineCount = 0;

  char tmp[MSGL];
  bzero(tmp, MSGL);

  sprintf(tmp, "%s", buf);
  
  for (int i = 0; i < 32; ++i)
  {
    bzero(msg_queue[i], 128);
  }
  
  char *line = strtok(tmp, "\n");
  while (line) {
    sprintf(msg_queue[lineCount], "%s", line);
    lineCount++;
    line = strtok(0, "\n");
  }

  /*
  int i = 0;
  while ( (strcmp(msg_queue[i], "") != 0) && (i<32) ){
    printf("line %i :: %s\n", i, msg_queue[i]); 
    i++;   
  }
  */

}

/**
 * Liest von einem Socket und verarbeitet die erhaltene Nachricht.
 *
 * sock: Der Socket, von dem gelesen werden soll.
 */

void parseMessages(int sock)
{

  int breaker = 1;

  char in_buf[MSGL];
  char out_buf[MSGL];

  bzero(in_buf, MSGL);
  bzero(out_buf, MSGL);

  printf("\n");

  while(breaker)
  {

    get_message(sock, in_buf);
    processMessage(in_buf);

    int linenum = 0;
    
    while(strcmp(msg_queue[linenum], "") != 0)
    {
      switch (msg_queue[linenum][0]) {
      
        case '+':

          if(strcmp(msg_queue[linenum], "+ MNM Gameserver v1.0 accepting connections") == 0) {
            
            sprintf(out_buf, "VERSION %0.1f\n", CVERSION);
            send_message(sock, out_buf);

          }else if(strcmp(msg_queue[linenum], "+ Client version accepted - please send Game-ID to join") == 0) {

            sprintf(out_buf, "ID %s\n", id);
            send_message(sock, out_buf);

          }else if(strcmp(msg_queue[linenum], "+ PLAYING NMMorris") == 0) {

            if(strcmp(msg_queue[linenum + 1], "") != 0){
              linenum++;
              printf("Spielname: %s\n", msg_queue[linenum]);
            }else{
              get_message(sock, in_buf);
              processMessage(in_buf);
            }
            sprintf(out_buf, "PLAYER\n");
            send_message(sock, out_buf);
            
          }else if(strstr(msg_queue[linenum], "+ YOU") != NULL) {

            printf("!!!YOU!!!\n");
                      
          }else if(strstr(msg_queue[linenum], "+ TOTAL") != NULL) {

            printf("!!!TOTAL!!!\n");
                      
          }else{
            printf("Unexpected message: %s\n", msg_queue[linenum]);
            breaker = 0;  
          }
          
          break;
        
        case '-':
          printf(RED);

          if(strcmp(msg_queue[linenum], "- No free computer player found for that game - exiting") == 0) {

            printf("Kein freier platz.\n");
                       
          }else if(strcmp(msg_queue[linenum], "- Socket timeout - please be quicker next time") == 0){

            printf("Socket timeout.\n");

          }else{
            printf("Unerwarteter Fehler.\n");
            breaker = 0;
          }

          break;
        
        default:
          printf("WAT\n");
          breaker = 0;
          break;
      }    

      linenum++;

    }

    printf(RESET);

  }

}