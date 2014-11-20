#include "connector.h"

char id[11];
char msg_queue[32][128];
char tokens[32][32];

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
 * Splitet eine line an leerzeichen und schreibt das Ergebniss in tokens.
 *
 * line: Zu splitende line.
 */

void tokenizeLine(char *line)
{

  int tokenCount = 0;

  char tmp[strlen(line)];

  for (int i = 0; i < 32; ++i)
  {
    bzero(tokens[i], 32);
  }

  sprintf(tmp, "%s", line);

  char *token = strtok(tmp, " ");
  while (token) {
    sprintf(tokens[tokenCount], "%s", token);
    tokenCount++;
    token = strtok(0, " ");
  }

  int i = 0;
  while( (strcmp(tokens[i], "") != 0) && i < 32){
    printf("token:: %s\n", tokens[i]);
    i++;
  }

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

  int my_pos;
  char my_name[20];
  int total_players;
  int max_move_time;

  printf("\n");

  while(breaker)
  {

    get_message(sock, in_buf);
    processMessage(in_buf);

    int linenum = 0;
    
    while( (strcmp(msg_queue[linenum], "") != 0) && linenum < 32)
    {
      switch (msg_queue[linenum][0]) {
      
        case '+':

          if(strcmp(msg_queue[linenum], "+ WAIT") == 0) {
            
            sprintf(out_buf, "OKWAIT\n");
            send_message(sock, out_buf);

          }else if(strcmp(msg_queue[linenum], "+ MNM Gameserver v1.0 accepting connections") == 0) {
            
            sprintf(out_buf, "VERSION %0.1f\n", CVERSION);
            send_message(sock, out_buf);

          }else if(strcmp(msg_queue[linenum], "+ Client version accepted - please send Game-ID to join") == 0) {

            sprintf(out_buf, "ID %s\n", id);
            send_message(sock, out_buf);

          }else if(strcmp(msg_queue[linenum], "+ PLAYING NMMorris") == 0) {

            if(strcmp(msg_queue[linenum + 1], "") != 0){
              linenum++;
              char gameid[20];
              sscanf(msg_queue[linenum], "+ %[^\t\n]", gameid);
              printf("Spielname: %s\n\n",gameid);
            }else{
              get_message(sock, in_buf);
              processMessage(in_buf);
              printf("Spielname: %s\n\n",msg_queue[0]);
            }
            sprintf(out_buf, "PLAYER\n");
            send_message(sock, out_buf);
            
          }else if(sscanf(msg_queue[linenum], "+ YOU %d %[^\t\n]", &my_pos, my_name) == 2) {

            printf("My position: %d\n", my_pos);
            printf("My ID: %s\n\n", my_name);

          }else if(sscanf(msg_queue[linenum], "+ TOTAL %d", &total_players) == 1) {

            printf("Total Players: %d\n\n", total_players);

            int opponent_pos;
            char opponent_name[20];
            int opponent_ready;
            int ret = sscanf(msg_queue[linenum+1], "+ %d %[A-Za-z0-9 ] %d", &opponent_pos, opponent_name, &opponent_ready);
            printf("Opponent position: %d\n", opponent_pos);
            printf("Opponent ID: %s\n", opponent_name);
            printf("Opponent ready?: %d\n", opponent_ready);
            printf("sscanf ret: %d\n\n", ret);
                      
          }else if(sscanf(msg_queue[linenum], "+ MOVE %d", &max_move_time) == 1) {

            printf("Maximum move time: %d\n\n", max_move_time);
                      
          }else if(strstr(msg_queue[linenum], "+ CAPTURE") != NULL) {

            printf("!!!CAPTURE!!!\n");            
                      
          }else{
        
            printf("Unexpected message: %s\n", msg_queue[linenum]);
        
          }
          
          break;
        
        case '-':
          printf(RED);

          if(strcmp(msg_queue[linenum], "- No free computer player found for that game - exiting") == 0) {

            printf("Kein freier platz.\n");
                       
          }else if(strcmp(msg_queue[linenum], "- Socket timeout - please be quicker next time") == 0){

            printf("Socket timeout.\n");

          }else if(strcmp(msg_queue[linenum], "- Protocol mismatch - you probably didn't want to talk to the fabulous gameserver") == 0){

            printf("Wrong input format.\n");

          }else{
            printf("Unexpected server error.\n");
          }

          breaker = 0;
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