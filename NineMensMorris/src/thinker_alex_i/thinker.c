#include "thinker.h"
#include "../config.h"
#include "../logger/logger.h"

void start_thinking(){
    kill (getppid(), SIGUSR1);
}

char* read_from_pipe(int *fd){

   static char buffer[ANSWERLENGTH];
   memset(buffer, 0, ANSWERLENGTH);
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

char* convert_pos_to_string(int fst_coord, int snd_coord){
  
  char *coords = malloc (2*sizeof(char*));
  
  switch(fst_coord) {
    case 0: sprintf(coords, "A%d", snd_coord); break;
    case 1: sprintf(coords, "B%d", snd_coord); break;
    case 2: sprintf(coords, "C%d", snd_coord); break;
  }
  
  return coords;
}


token check_counter_clock_wise(plist_struct *plist_str, int fst_coord, int snd_coord){

    token cc_wise;
    cc_wise.x = fst_coord;
    cc_wise.y = (snd_coord + 7) % 8;
    cc_wise.value = plist_str -> piece_list[cc_wise.x][cc_wise.y];

    return cc_wise;
}

token check_clock_wise(plist_struct *plist_str, int fst_coord, int snd_coord){

    token c_wise;
    c_wise.x = fst_coord;
    c_wise.y = (snd_coord + 1) % 8;
    c_wise.value = plist_str -> piece_list[c_wise.x][c_wise.y];

    return c_wise;
}
  
  

/**
 * !fst_coord==0    <==> Nicht im A Level
 * snd_coord % 2    <==> Nur "kreuz" Koordinaten (1,3,5,7)
 */
token check_level_up(plist_struct *plist_str, int fst_coord, int snd_coord){

    token up;

    switch ((!(fst_coord==0)) && snd_coord % 2){
      case 0: 
               up.x = fst_coord;
               up.y = snd_coord;
               up.value = 3;
               return up;
      break;
      case 1:  up.x = fst_coord-1;
               up.y = snd_coord;
               up.value = plist_str -> piece_list[up.x][up.y];
               return up;
      break;
      default: printf("Dies sollte niemals passieren!");
               return up;

    }
    
}

/**
 * !fst_coord==0    <==> Nicht im C Level
 * snd_coord % 2    <==> Nur "kreuz" Koordinaten (1,3,5,7)
 */
token check_level_down(plist_struct *plist_str, int fst_coord, int snd_coord){

    token down;

    switch ((!(fst_coord==2)) && (snd_coord % 2)==1){
      case 0:  
               down.x = fst_coord;
               down.y = snd_coord;
               down.value = 3;
               return down;
      break;
      case 1:  down.x = fst_coord+1;
               down.y = snd_coord;
               down.value = plist_str -> piece_list[down.x][down.y];
               return down;
      break;
      default: printf("Dies sollte niemals passieren!");
               return down;
    }
}

neighbors_struct get_neighbors(plist_struct *plist_str){
    
    neighbors_struct wrap;
    int length = 0;
      for(int x = 0; x<3; x++){
       for(int y = 0; y<8; y++){
         if(plist_str -> piece_list[x][y] == 1){
            wrap.array_neighbors[length].x = x;
            wrap.array_neighbors[length].y = y;
            wrap.array_neighbors[length].neighbors[0] = check_clock_wise(plist_str, x, y).value;
            wrap.array_neighbors[length].neighbors[1] = check_counter_clock_wise(plist_str, x, y).value;
            wrap.array_neighbors[length].neighbors[2] = check_level_up(plist_str, x, y).value;
            wrap.array_neighbors[length].neighbors[3] = check_level_down(plist_str, x, y).value;
            length++;
         }
       }
     }
     wrap.length = length;

return wrap;

}

void print_neighbors(neighbors_struct wrap){

   for (int i = 0; i < wrap.length; i++){
      printf ("x = %i | y = %i \n", wrap.array_neighbors[i].x,wrap.array_neighbors[i].y);
      printf ("[0] = %i | [1] = %i | [2] = %i | [3] = %i \n", wrap.array_neighbors[i].neighbors[0], wrap.array_neighbors[i].neighbors[1],wrap.array_neighbors[i].neighbors[2],wrap.array_neighbors[i].neighbors[3]);
   }
}

char* get_enemy_piece(plist_struct *plist_str){

    srand(time(NULL));
    
    int counter = 0;
    int rnd = (rand() % plist_str -> countEnemyPieces+1);
    
    char *answer = malloc (2*sizeof(char*));
  //  free(answer);
    printf("MILL answer should be NULL = %s\n", answer);

    for(int x = 0; x<3; x++){
      for(int y = 0; y<8; y++){
        if(plist_str -> piece_list[x][y] == 2){
          counter++;
          if(counter == rnd){
             answer = convert_pos_to_string(x,y);
          }
        }  
      }
    }

    return answer;
}

char* set_phase(plist_struct *plist_str){

    srand(time(NULL));
    
    int counter = 0;
    int count_free_spaces = 24 - plist_str -> countMyPieces - plist_str -> countEnemyPieces;
    int rnd = (rand() % count_free_spaces+1);

    char *answer = malloc (ANSWERLENGTH * sizeof(char*));
    //free(answer);

    for(int x = 0; x<3; x++){
      for(int y = 0; y<8; y++){
        if(plist_str -> piece_list[x][y] == 0){
          counter++;
          if(counter == rnd){
            answer = convert_pos_to_string(x,y);
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

    char *answer = malloc (ANSWERLENGTH *sizeof(char*));
 

      if(plist_str -> unplacedPieces == 9){
          printf("convPos(0,0) = %s\n", convert_pos_to_string(0,0));
          answer = convert_pos_to_string(0,0);
          plist_str -> unplacedPieces--;
      }
      else if(plist_str -> unplacedPieces == 8){
          answer = convert_pos_to_string(0,1);
          plist_str -> unplacedPieces--;
      }
      else if(plist_str -> unplacedPieces == 7){
          answer = convert_pos_to_string(0,2);
          printf("Mill 3rd & answer = %s\n", answer);
          plist_str -> unplacedPieces--;
      }
      else{
          answer = set_phase(plist_str);
      }

    return answer;
}

// Kopiert aus http://stackoverflow.com/questions/6127503/shuffle-array-in-c
void shuffle(points_struct *array_neighbors, int length){
    
    if (length > 1){
        
        for (int i = 0; i < length - 1; i++){

          int j = i + rand() / (RAND_MAX / (length - i) + 1);
          points_struct t = array_neighbors[j];
          array_neighbors[j] = array_neighbors[i];
          array_neighbors[i] = t;
        }
    }
}

char* slide_phase(plist_struct *plist_str, neighbors_struct wrap){


      token to;
      srand(time(NULL));
      char *answer = malloc (ANSWERLENGTH*sizeof(char*));
     // free(answer);
      char *answer_from = malloc (2*sizeof(char*));
      //free(answer_from);
      char *answer_to   = malloc (2*sizeof(char*));
      //free(answer_to);

      shuffle(wrap.array_neighbors, wrap.length);

      for (int i = 0; i < wrap.length; i++){
        for (int j = 0; j < 4; j++){
          if(wrap.array_neighbors[i].neighbors[j] == 0){

            answer_from = convert_pos_to_string(wrap.array_neighbors[i].x, wrap.array_neighbors[i].y);

            switch(j){
              case 0: to = check_clock_wise(plist_str, wrap.array_neighbors[i].x, wrap.array_neighbors[i].y);
                      answer_to = convert_pos_to_string(to.x, to.y);
              break; 
              case 1: to = check_counter_clock_wise(plist_str, wrap.array_neighbors[i].x, wrap.array_neighbors[i].y);
                      answer_to = convert_pos_to_string(to.x, to.y);
              break; 
              case 2: to = check_level_up(plist_str, wrap.array_neighbors[i].x, wrap.array_neighbors[i].y);
                      answer_to = convert_pos_to_string(to.x, to.y);
              break; 
              case 3: to = check_level_down(plist_str, wrap.array_neighbors[i].x, wrap.array_neighbors[i].y);
                      answer_to = convert_pos_to_string(to.x, to.y);
              break; 
              default: printf("Error while setting neighbor!\n"); 
              break;
            }

          }
        }
      }

      sprintf(answer, "%s:%s", answer_from, answer_to);
      return answer;
}


char* jump_phase(plist_struct *plist_str, neighbors_struct wrap){

      srand(time(NULL));

      char *answer = malloc (ANSWERLENGTH*sizeof(char*));
     // free(answer);
      printf("JUMP answer should be NULL = %s\n", answer);  // why does this happen
      char *answer_from = malloc (2*sizeof(char*));
     // free(answer_from);
      char *answer_to   = malloc (2*sizeof(char*));
     // free(answer_to);
 
     
      int count_free_spaces = 24 - plist_str -> countMyPieces - plist_str -> countEnemyPieces;
      int rnd_token = rand() % 3;
      int rnd_spaces = rand() % count_free_spaces;
      int counter = 0;

      for (int i = 0; i < wrap.length; i++){
        if(i == rnd_token){
           answer_from = convert_pos_to_string(wrap.array_neighbors[i].x, wrap.array_neighbors[i].y);
        }
      }  
    
      for(int x = 0; x<3; x++){
        for(int y = 0; y<8; y++){
          if(plist_str -> piece_list[x][y] == 0){
                if(counter == rnd_spaces){
                    answer_to = convert_pos_to_string(x,y);
                    x = 3;  // not happy with this solution
                    y = 8;  // not happy with this solution
                }
             counter++;
          }
        }
      }
    sprintf(answer, "%s:%s", answer_from, answer_to);

    return answer;
}


void calc_turn(shm_struct *shm_str, plist_struct *plist_str, int shm_id, int plist_id, int *fd){
  
    if (!check_think_flag(shm_str)){end_routine(shm_str, plist_str, shm_id, plist_id);}

    char *answer = NULL;

    neighbors_struct wrap;
    wrap = get_neighbors(plist_str);

    //print_neighbors(wrap);

    //0.Mühlenfall: Wenn eine Mühle vorhanden ist, müssen gegnerische Steine geschlagen werden
    if(plist_str -> piecesToRemove > 0){
      printf("I'm in "GREEN "Mill" RESET "-Phase!\n");
      answer = get_enemy_piece(plist_str);
    }

    //1. Setzphase: Es dürfen nicht alle Steine verteilt sein und kein Stein muss geschlagen werden
    else if(plist_str -> unplacedPieces > 0 && plist_str -> piecesToRemove == 0){
      printf("I'm in "GREEN "Set" RESET "-Phase!\n");
      answer = set_phase(plist_str);
      //answer = set_sure_mill(plist_str);
    }

    //2. Schieb-Phase: Es müssen alle Steine verteilt sein, die KI muss mehr als 3 Steine besitzen
    //                 und kein Stein muss geschlagen werden
    else if(plist_str -> unplacedPieces == 0 && plist_str -> countMyPieces > 3 && plist_str -> piecesToRemove == 0){
      printf("I'm in " GREEN "Slide" RESET "-Phase!\n");
      answer = slide_phase(plist_str, wrap);
    }



    //3. Sprungphase: Es müssen alle Steine verteilt sein und die KI muss genau 3 oder weniger Steine besitzen
    else if(plist_str -> unplacedPieces == 0 && plist_str -> countMyPieces <= 3){
      printf("I'm in " GREEN "Jump" RESET "-Phase!\n");
      answer = jump_phase(plist_str, wrap);
    }
    //4. Fehlerfall: 
    else {
      printf(RED "Thinker konnte keine Phase erkennen!\n");
    }

    //5. Endroutine: Steinecount für beide Spieler zurücksetzen, Flag zurücksetzen, Feld leeren
    //               Antwort schicken

    plist_str -> countMyPieces = 0;
    plist_str -> countEnemyPieces = 0;
    plist_str -> piecesToRemove = 0;  // Fall für 2 Steine muss noch abgefangen werden

    set_think_flag(false,shm_str);

    memset(plist_str -> piece_list, 0, 3*8*sizeof(int));
    if(!write_to_pipe(fd, answer)){end_routine(shm_str, plist_str, shm_id, plist_id);}
    free(answer);
}

