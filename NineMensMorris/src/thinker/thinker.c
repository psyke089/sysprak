#include "thinker.h"
#include "../config.h"
#include "../logger/logger.h"
//#include "../shm/shmManager.h"

void start_thinking(){
    kill (getppid(), SIGUSR1);
}

char* read_from_pipe(int *fd){

   static char buffer[ANSWERLENGTH];
   char *log_msg;
   int bytes;

   if ((bytes = read(fd[READ], buffer, ANSWERLENGTH)) <= 0) {
        logPrnt('r', 'e', "\nCouldn't read from pipe!\n");
        memset(buffer, 0, ANSWERLENGTH);
   }
   else {
      asprintf(&log_msg, "\nRead %i bytes from the pipe: %s\n", bytes, buffer);
      logPrnt('g', 'p', log_msg); 
   }

   return buffer;

}


int write_to_pipe(int *fd, char *str){
 
   if (str == NULL || write(fd[WRITE], str, strlen(str)) <= 0) {
        logPrnt('r', 'e', "\nCouldn't write to pipe!\n");
        return 0;
   }
   return 1;
}

char* convertPositionToString(int fst_coord, int snd_coord){
  static char coords[2];

  switch(fst_coord) {
    case 0: coords[0] = 'A'; break;
    case 1: coords[0] = 'B'; break;
    case 2: coords[0] = 'C'; break;
  }

  coords[1] = (char)(((int)'0')+snd_coord);

  return coords;
}

neighbors_struct get_neighbors(plist_struct *plist_str){
    
    neighbors_struct wrap;
    //0 Links (y-1) im Ring
    //1 Recht (y+1) im Ring
    //2 Innerer Ringnachbar (x+1)
    //3 Äußerer Ringnachbar (x-1) im Ring
    //memset(wrap.arrayOfNeighbors[3][8][4], 0, NEIGHBOR_SIZE);

    //Linux Fehler "set but not used"
    if(wrap.arrayOfNeighbors[0][0][0]==0){wrap.arrayOfNeighbors[0][0][0] = 0;}

    //arrayOfNeighbors mit Nachbarn befüllen
    for(int x = 0; x<3; x++){
      for(int y = 0; y<8; y++){
        if (plist_str -> piece_list[x][7]!=0){
          wrap.arrayOfNeighbors[x][0][0] = plist_str -> piece_list[x][7];
        }

        if (plist_str -> piece_list[x][0]!=0){
          wrap.arrayOfNeighbors[x][7][1] = plist_str -> piece_list[x][0];
        }

        if (y > 0 && plist_str -> piece_list[x][y-1]!=0){

          wrap.arrayOfNeighbors[x][y][0] = plist_str -> piece_list[x][y-1];
        }

        if (y < 7 && plist_str -> piece_list[x][y+1]!=0){
          wrap.arrayOfNeighbors[x][y][1] = plist_str -> piece_list[x][y+1];
        }

      }
    }

    //Brückenfälle

    //Setze alle arrayOfNeighbors oben und unten auf 3
    for(int x = 0; x<3; x++){
      for(int y = 0; y<8; y++){
      wrap.arrayOfNeighbors[x][y][2] = 3;
      wrap.arrayOfNeighbors[x][y][3] = 3;
      }
    }

    //A1
      wrap.arrayOfNeighbors[1][1][3] = plist_str -> piece_list[0][1];

    //A3
      wrap.arrayOfNeighbors[1][3][3] = plist_str -> piece_list[0][3];

    //A5
      wrap.arrayOfNeighbors[1][5][3] = plist_str -> piece_list[0][5];

    //A7
      wrap.arrayOfNeighbors[1][7][3] = plist_str -> piece_list[0][7];


    //C1 
      wrap.arrayOfNeighbors[1][1][2] = plist_str -> piece_list[2][1];

    //C3
      wrap.arrayOfNeighbors[1][3][2] = plist_str -> piece_list[2][3];

    //C5
      wrap.arrayOfNeighbors[1][5][2] = plist_str -> piece_list[2][5];

    //C7
      wrap.arrayOfNeighbors[1][7][2] = plist_str -> piece_list[2][7];


    //B1
      wrap.arrayOfNeighbors[0][1][2] = plist_str -> piece_list[1][1];
      wrap.arrayOfNeighbors[2][1][3] = plist_str -> piece_list[1][1];

    //B3
      wrap.arrayOfNeighbors[0][3][2] = plist_str -> piece_list[1][3];
      wrap.arrayOfNeighbors[2][3][3] = plist_str -> piece_list[1][3];

    //B5
      wrap.arrayOfNeighbors[0][5][2] = plist_str -> piece_list[1][5];
      wrap.arrayOfNeighbors[2][5][3] = plist_str -> piece_list[1][5];

    //B7
      wrap.arrayOfNeighbors[0][7][2] = plist_str -> piece_list[1][7];
      wrap.arrayOfNeighbors[2][7][3] = plist_str -> piece_list[1][7];

return wrap;

}

void print_neighbors(plist_struct *plist_str, neighbors_struct wrap){

     for(int x = 0; x<3; x++){
       for(int y = 0; y<8; y++){
         for(int z = 0; z<4; z++){
           printf("piece_list[%i][%i]: %i arrayOfNeighbors[%i][%i][%i]: %i \n",x,y,plist_str -> piece_list[x][y],x,y,z,wrap.arrayOfNeighbors[x][y][z]);
         }
       }
     }
}

char* get_enemy_piece(plist_struct *plist_str){

    srand(time(NULL));
    
    int counter = 0;
    int rnd = (rand() % plist_str -> countEnemyPieces)+1;
    static char answer[ANSWERLENGTH];

    for(int x = 0; x<3; x++){
      for(int y = 0; y<8; y++){
        if(plist_str -> piece_list[x][y] == 2){
          counter++;
          if(counter == rnd){
            strcpy(answer, convertPositionToString(x,y));
            break;
          }
        }  
      }
    }
    return answer;
}

char* set_phase(plist_struct *plist_str){

    srand(time(NULL));
    
    int counter = 0;
    int countFreeSpaces = 24 - plist_str -> countMyPieces + plist_str -> countEnemyPieces;
    int rnd = (rand() % countFreeSpaces)+1;
    static char answer[ANSWERLENGTH];

    for(int x = 0; x<3; x++){
      for(int y = 0; y<8; y++){
        if(plist_str -> piece_list[x][y] == 0){
          counter++;
          if(counter == rnd){
            strcpy(answer, convertPositionToString(x,y));
            plist_str -> unplacedPieces--;
            break;
          }
        }  
      }
    }

    return answer;
}


/**
 * Erster Zug auf A0
 * Zweiter Zug auf A1
 * Dritter Zug auf A2
 */
char* set_sure_mill(plist_struct *plist_str){

    static char answer[ANSWERLENGTH];

      if(plist_str -> unplacedPieces == 9){
          strcpy(answer,convertPositionToString(0,0));
          plist_str -> unplacedPieces--;
      }
      else if(plist_str -> unplacedPieces == 8){
          strcpy(answer,convertPositionToString(0,1));
          plist_str -> unplacedPieces--;
      }
      else if(plist_str -> unplacedPieces == 7){
          strcpy(answer,convertPositionToString(0,2));
          plist_str -> unplacedPieces--;
      }
      else{
        strcpy(answer, set_phase(plist_str));
      }

    return answer;
}

char* slide_phase(plist_struct *plist_str, neighbors_struct wrap){

      srand(time(NULL));
      static char answer[ANSWERLENGTH];
      point useful[8];
      int counter = 0;

            for(int x = 0; x<3; x++){
              for(int y = 0; y<8; y++){
                if(plist_str -> piece_list[x][y] == 1){

                   if (wrap.arrayOfNeighbors[x][y][0] == 0) {
                       useful[counter].x = x;
                       useful[counter].y = y;
                       useful[counter].neighbors[0] = 1;
                    }
                    if (wrap.arrayOfNeighbors[x][y][1] == 0) {
                       useful[counter].x = x;
                       useful[counter].y = y;
                       useful[counter].neighbors[1] = 1;
                    }
                    if (wrap.arrayOfNeighbors[x][y][2] == 0) {
                       useful[counter].x = x;
                       useful[counter].y = y;
                       useful[counter].neighbors[2] = 1;
                    }
                    if (wrap.arrayOfNeighbors[x][y][3] == 0) {
                       useful[counter].x = x;
                       useful[counter].y = y;
                       useful[counter].neighbors[3] = 1;
                    } 
                    counter++;
                }
              }
            }
            

      int rnd = (rand() % counter)+1;

      int old_x = useful[rnd].x;
      int old_y = useful[rnd].y;

      strcpy(answer,convertPositionToString(old_x,old_y));
      strcat(answer,":");

            if(useful[rnd].neighbors[0]){
              int new_y = !old_y ? 7:old_y-1;
              strcat(answer, convertPositionToString(old_x, new_y));
            }
            else if(useful[rnd].neighbors[1]){
              int new_y = old_y==7 ? 0:old_y+1;
              strcat(answer,convertPositionToString(old_x,new_y));
            }
            else if(useful[rnd].neighbors[2]){
              int new_x = old_x+1;
              strcat(answer,convertPositionToString(new_x,old_y));
            }
            else{
              int new_x = old_x-1;
              strcat(answer,convertPositionToString(new_x,old_y));
            }
      return answer;
}


char* jump_phase(plist_struct *plist_str){

      srand(time(NULL));

      static char answer[ANSWERLENGTH];
      int countFreeSpaces = 24 - plist_str -> countMyPieces + plist_str -> countEnemyPieces;
      int rnd = (rand() % countFreeSpaces)+1;
      int counter = 0;

      for(int x = 0; x<3; x++){
        for(int y = 0; y<8; y++){
          if(plist_str -> piece_list[x][y] == 1){
            strcpy(answer,convertPositionToString(x,y));
            strcat(answer,":");
            break;
          }
        }
      }


      for(int x = 0; x<3; x++){
        for(int y = 0; y<8; y++){
          if(plist_str -> piece_list[x][y] == 0){
            counter++;
             if(counter == rnd){
                strcat(answer,convertPositionToString(x,y));
                break;
             }
          }   
        }
      }
    return answer;
}


void calc_turn(shm_struct *shm_str, plist_struct *plist_str, int shm_id, int plist_id, int *fd){
  
    if (!check_think_flag(shm_str)){end_routine(shm_str, plist_str, shm_id, plist_id);}

    char *answer = malloc (ANSWERLENGTH * sizeof(char*));

    neighbors_struct wrap;
    wrap = get_neighbors(plist_str);

    print_neighbors(plist_str, wrap);

    //0.Mühlenfall: Wenn eine Mühle vorhanden ist, müssen gegnerische Steine geschlagen werden
    if(plist_str -> piecesToRemove > 0){
      strcpy(answer, get_enemy_piece(plist_str));
    }

    //1. Setzphase: Es dürfen nicht alle Steine verteilt sein und kein Stein muss geschlagen werden
    if(plist_str -> unplacedPieces > 0 && plist_str -> piecesToRemove == 0){
      strcpy(answer, set_phase(plist_str));
      //strcpy(answer, set_sure_mill(plist_str));
    }

    //2. Schieb-Phase: Es müssen alle Steine verteilt sein, die KI muss mehr als 3 Steine besitzen
    //                 und kein Stein muss geschlagen werden
    if(plist_str -> unplacedPieces == 0 && plist_str -> countMyPieces > 3 && plist_str -> piecesToRemove == 0){
      strcpy(answer, slide_phase(plist_str, wrap));
    }



    //3. Sprungphase: Es müssen alle Steine verteilt sein und die KI muss genau 3 oder weniger Steine besitzen
    if(plist_str -> unplacedPieces == 0 && plist_str -> countMyPieces <= 3){
      strcpy(answer, jump_phase(plist_str));
    }

    //4. Endroutine: Steinecount für beide Spieler zurücksetzen, Flag zurücksetzen, Feld leeren
    //               Antwort schicken, Antwort leeren

    plist_str -> countMyPieces = 0;
    plist_str -> countEnemyPieces = 0;
    plist_str -> piecesToRemove = 0;  // Fall für 2 Steine muss noch abgefangen werden

    set_think_flag(false,shm_str);

    memset(plist_str -> piece_list, 0, 3*8*sizeof(int));
    
    if(!write_to_pipe(fd, answer)){end_routine(shm_str, plist_str, shm_id, plist_id);}

    free(answer);

}

