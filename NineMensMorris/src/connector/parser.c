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

  /*
  int i = 0;
  while( (strcmp(tokens[i], "") != 0) && i < 32){
    printf("token:: %s\n", tokens[i]);
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

  int total_players;
  int max_move_time;
  int captured_pieces;

  int pl_players;
  int pl_pieces;

  int my_pos;
  char my_name[20];

  int opponent_pos;
  char opponent_name[20];
  int opponent_ready;

  int piece_player;
  int piece_id;
  char piece_pos[2];

  printf("\n");

  while(breaker)
  {

    get_message(sock, in_buf);
    processMessage(in_buf);

    int linenum = 0;
    
    while( (strcmp(msg_queue[linenum], "") != 0) && linenum < 32)
    {
      switch (msg_queue[linenum][0]) {
        
        /**
         *  G A M E   L O G I C
         */

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

            if(strcmp(msg_queue[linenum + 1], "") != 0){
              linenum++;
              tokenizeLine(msg_queue[linenum]);
              
              sscanf(tokens[1], "%d", &opponent_pos);

              bzero(opponent_name, 20);
              int i = 2;            
              while( strcmp(tokens[i+1], "") != 0){
                strcat(tokens[i], " ");
                strcat(opponent_name, tokens[i]);
                i++;
              }
                
              opponent_ready = atoi(tokens[i]);

              printf("Opponent position: %d\n", opponent_pos);
              printf("Opponent ID: %s\n", opponent_name);
              if(opponent_ready == 1){
                printf("Opponent ready?: YES\n\n");
              }else{
                printf("Opponent ready?: NO\n\n");
              }

            }
                      
          }else if(strcmp(msg_queue[linenum], "+ ENDPLAYERS") == 0) {

            //nothing to do...

          }else if(sscanf(msg_queue[linenum], "+ MOVE %d", &max_move_time) == 1) {

            printf("Maximum move time: %d\n\n", max_move_time);
                      
          }else if(sscanf(msg_queue[linenum], "+ CAPTURE %d", &captured_pieces) == 1) {

            printf("Captured pieces: %d\n\n", captured_pieces);
                      
          }else if(sscanf(msg_queue[linenum], "+ PIECELIST %d,%d", &pl_players, &pl_pieces) == 2) {

            printf("Piecelist for %d players and %d pieces per player:\n\n", pl_players, pl_pieces);

          }else if(sscanf(msg_queue[linenum], "+ PIECE%d.%d %[A-C0-7]", &piece_player, &piece_id, piece_pos) == 3) {
            
            printf("Player %d, piece %d at position %s\n", piece_player, piece_id, piece_pos);

          }else if(strcmp(msg_queue[linenum], "+ ENDPIECELIST") == 0) {

            sprintf(out_buf, "THINKING \n");
            send_message(sock, out_buf);

          }else if(strcmp(msg_queue[linenum], "+ OKTHINK") == 0) {

            printf("thinking...\n");

          }else{
        
            printf(YELLOW "Unexpected message: %s\n" RESET, msg_queue[linenum]);
        
          }
          
          break;
        
        /**
         *  E R R O R   H A N D L I N G
         */

        case '-':
          printf(RED);

          if(strcmp(msg_queue[linenum], "- No free computer player found for that game - exiting") == 0) {

            printf("Kein freier platz.\n");
                       
          }else if(strcmp(msg_queue[linenum], "- Socket timeout - please be quicker next time") == 0){

            printf("Socket timeout.\n");

          }else if(strcmp(msg_queue[linenum], "- Protocol mismatch - you probably didn't want to talk to the fabulous gameserver") == 0){

            printf("Wrong input format.\n");

          }else if(strcmp(msg_queue[linenum], "- We expected you to THINK!") == 0){

            printf("Server expects client to think.\n");

          }else{
            printf("Unexpected server error: %s\n", msg_queue[linenum]);
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