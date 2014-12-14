#include "../main.h"
#include "../thinker/thinker.h"
#include "../config.h"
#include "connector.h"

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

void parseMessages(int sock, shm_struct *shm_str, plist_struct *plist_str, int *pipe_fd)
{

  int breaker = 1;

  char in_buf[MSGL];
  char out_buf[MSGL];

  memset(in_buf, 0, MSGL);
  memset(out_buf, 0, MSGL);

  int total_players;
  int max_move_time;
  int captured_pieces;
  char game_type[20];

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

  int snd_coord = 0;
  int fst_coord = 0;
  snd_coord++;
  fst_coord++;

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

            sprintf(out_buf, "ID %s\n", shm_str -> gameID);
            send_message(sock, out_buf);

          }else if(strcmp(msg_queue[linenum], "+ PLAYING NMMorris") == 0) {

            if(strcmp(msg_queue[linenum + 1], "") != 0){
              linenum++;
              sscanf(msg_queue[linenum], "+ %[^\t\n]", game_type);
              printf("Game Name: %s\n\n",game_type);

              strcpy(shm_str -> gameName, game_type);

            }else{
              get_message(sock, in_buf);
              processMessage(in_buf);
              sprintf(game_type, "%s",msg_queue[0]);
              printf("Game Name: %s\n\n",game_type);

              strcpy(shm_str -> gameName, game_type);

            }
            sprintf(out_buf, "PLAYER\n");
            send_message(sock, out_buf);

          }else if(sscanf(msg_queue[linenum], "+ YOU %d %[^\t\n]", &my_pos, my_name) == 2) {

            printf("My position: %d\n", my_pos);
            printf("My Name: %s\n\n", my_name);

            shm_str -> player_str[0].playerID = my_pos;
            strcpy(shm_str -> player_str[0].playerName,  my_name);
            shm_str -> player_str[0].isReady =  1;
            shm_str -> player_str[0].isLoggedIn =  1;
          
          }else if(sscanf(msg_queue[linenum], "+ TOTAL %d", &total_players) == 1) {

            printf("Total Players: %d\n\n", total_players);
            shm_str -> playerCount = total_players;

            if(strcmp(msg_queue[linenum + 1], "") != 0){
              linenum++;
              tokenizeLine(msg_queue[linenum]);

              sscanf(tokens[1], "%d", &opponent_pos);

              memset(opponent_name, 0, 20);
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
            shm_str -> player_str[1].playerID = opponent_pos;
            strcpy(shm_str -> player_str[1].playerName,  opponent_name);
            shm_str -> player_str[1].isReady =  opponent_ready;
            shm_str -> player_str[1].isLoggedIn =  1;
            }

          }else if(strcmp(msg_queue[linenum], "+ ENDPLAYERS") == 0) {

            //nothing to do...

          }else if(sscanf(msg_queue[linenum], "+ MOVE %d", &max_move_time) == 1) {

            printf("Maximum move time: %d\n\n", max_move_time);

          }else if(sscanf(msg_queue[linenum], "+ CAPTURE %d", &captured_pieces) == 1) {

            printf("Captured pieces: %d\n\n", captured_pieces);
            plist_str -> piecesToRemove = captured_pieces;

          }else if(sscanf(msg_queue[linenum], "+ PIECELIST %d,%d", &pl_players, &pl_pieces) == 2) {

            printf("Piecelist for %d players and %d pieces per player:\n\n", pl_players, pl_pieces);

          }else if(sscanf(msg_queue[linenum], "+ PIECE%d.%d %[A-C0-7]", &piece_player, &piece_id, piece_pos) == 3) {
            

            if (my_pos == 0){ // wenn die KI der erste Spieler ist (dirty)
            
                if(strlen(piece_pos) == 2){
                  
                      fst_coord = piece_pos[0] - 65;
                      snd_coord = atoi(&piece_pos[1]); 
    
                          switch (piece_player){
                            case 0: // KI-Spielsteine werden mit 1 in der Liste gespeichert
                                    plist_str -> piece_list[fst_coord][snd_coord] = 1;
                                    plist_str -> countMyPieces++;
                            break; 
                            case 1: // Gegnerische Spielsteine werden mit 2 in der Liste gespeichert
                                    plist_str -> piece_list[fst_coord][snd_coord] = 2; 
                                    plist_str -> countEnemyPieces++;
                            break; 
                            default:printf("Für mehr Spieler noch erweitern!");
                            break;
                          }
                } 
             }
             else {       // wenn die KI der zweite Spieler ist (dirty)
                
                if(strlen(piece_pos) == 2){
                  
                      fst_coord = piece_pos[0] - 65;
                      snd_coord = atoi(&piece_pos[1]); 
    
                          switch (piece_player){
                            case 1: // KI-Spielsteine werden mit 1 in der Liste gespeichert
                                    plist_str -> piece_list[fst_coord][snd_coord] = 1;
                                    plist_str -> countMyPieces++;
                            break; 
                            case 0: // Gegnerische Spielsteine werden mit 2 in der Liste gespeichert
                                    plist_str -> piece_list[fst_coord][snd_coord] = 2; 
                                    plist_str -> countEnemyPieces++;
                            break; 
                            default:printf("Für mehr Spieler noch erweitern!");
                            break;
                          }
                } 
             }

            printf("Player %d, piece %d at position %s\n", piece_player, piece_id, piece_pos);

          }else if(strcmp(msg_queue[linenum], "+ ENDPIECELIST") == 0) {

            sprintf(out_buf, "THINKING \n");
            send_message(sock, out_buf);

          }else if(strcmp(msg_queue[linenum], "+ OKTHINK") == 0) {

            printf("thinking...\n");
            
            set_think_flag(true, shm_str);
            start_thinking();

            //warte auf die antwort

            sprintf(out_buf, "PLAY %s\n", read_from_pipe(pipe_fd));

            send_message(sock, out_buf);

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

            printf("No free seat.\n");

          }else if(strcmp(msg_queue[linenum], "- Socket timeout - please be quicker next time") == 0){

            printf("Socket timeout.\n");

          }else if(strcmp(msg_queue[linenum], "- Protocol mismatch - you probably didn't want to talk to the fabulous gameserver") == 0){

            printf("Wrong input format.\n");

          }else if(strcmp(msg_queue[linenum], "- We expected you to THINK!") == 0){

            printf("Server expects client to think.\n");

          }else if(strcmp(msg_queue[linenum], "- Destination is already occupied") == 0){

            printf("Destination occupied.\n");

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
