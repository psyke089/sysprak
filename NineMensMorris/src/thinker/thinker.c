#include "thinker.h"
#include "../logger/logger.h"
#include "../main.h"

void start_thinking(){
    kill (getppid(), SIGUSR1);
}

char* read_from_pipe(int *fd){

   static char buffer[ANSWERLENGTH];
   int bytes;

   if ((bytes = read(fd[READ], buffer, ANSWERLENGTH)) <= 0) {
        logPrnt('r', 'e', "\nCouldn't read from pipe!\n");
        memset(buffer, 0, ANSWERLENGTH);
   }
   else {
    
    //  asprintf invalid in C99 für mac
    //  asprintf(&log_msg, "\nRead %i bytes from the pipe: %s\n", bytes, buffer);
    char log_msg[50];
    sprintf(log_msg, "\nRead %i bytes from pipe!\n", bytes);
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


/* 
* falls man einen check auf auf mehrer seiten anwenden will
*/
int selectSide (plist_struct *paramStruct, int side, int circle, int stone){
    return (-1); // not implemented yet
}


// test für slctP
//plist_struct *test_plist; //@todo remove 

/* 
* returned das entsprechene piece an der Arraypostition
* piece_list[circle][num]
* zB: slctP(1,3)
*/
int slctP(int circle, int num) {
    if ( ( (circle < 0) || (circle > 3) ) || ( (num < 0) || (num > 7) ) ){
        perror("Piece coordinates out of bounds");
        return -1;
    }
    else{
    //return (*plist_struct).piece_list[circle][num]; //@todo anpassen 
    return -1; // noch auf passendes struct anpassen
    }
}

char* convertPositionToString(int x, int y){
  static char coords[2];

  switch(x) {
    case 0: coords[0] = 'A'; break;
    case 1: coords[0] = 'B'; break;
    case 2: coords[0] = 'C'; break;
  }

  //Cast digit zu char
  coords[1] = (char)(((int)'0')+y);

  return coords;
}

void calc_turn(shm_struct *shm_str, plist_struct *plist_str, int shm_id, int plist_id, int *fd){
  
    // think-flag muss überprüft werden
    if (!check_think_flag(shm_str)){end_routine(shm_str, plist_str, shm_id, plist_id);}

    // testoutput
    printf(GREEN "\nDer Beispielfill von plist_str -> countMyPieces ist = %i\n" RESET, plist_str -> countMyPieces);

    // Berechne die antwort und speichere sie in answer

    char answer[ANSWERLENGTH] = "A1";

    srand(time(NULL));

    int randomnumber;


    int arrayOfNeighbors[3][8][4] = {{{0}}};

    //Linux Fehler "set but not used"
    if(arrayOfNeighbors[0][0][0]==0){arrayOfNeighbors[0][0][0] = 0;}
    /*
    for(int x = 0; x<3; x++){
      for(int y = 0; y<8; y++){
        for(int z = 0; z<4; z++){
          arrayOfNeighbors[x][y][z] = 1;
        }
      }
    }

    for(int x = 0; x<3; x++){
      for(int y = 0; y<8; y++){
          plist_str -> piece_list[x][y] = 2;
      }
    }
    */

    //arrayOfNeighbors mit Nachbarn befüllen
    for(int x = 0; x<3; x++){
      for(int y = 0; y<8; y++){
        if (plist_str -> piece_list[x][7]!=0){
          arrayOfNeighbors[x][0][0] = plist_str -> piece_list[x][7];
        }

        if (plist_str -> piece_list[x][0]!=0){
          arrayOfNeighbors[x][7][1] = plist_str -> piece_list[x][0];
        }


        if (y > 0 && plist_str -> piece_list[x][y-1]!=0){
          arrayOfNeighbors[x][y][0] = plist_str -> piece_list[x][y-1];
        }

        if (y < 7 && plist_str -> piece_list[x][y+1]!=0){
          arrayOfNeighbors[x][y][1] = plist_str -> piece_list[x][y+1];
        }

      }
    }

    //A1
    if(plist_str -> piece_list[0][1]!=0){
      arrayOfNeighbors[1][1][2] = plist_str -> piece_list[0][1];
    }

    //A3
    if(plist_str -> piece_list[0][3]!=0){
      arrayOfNeighbors[1][3][2] = plist_str -> piece_list[0][3];
    }

    //A5
    if(plist_str -> piece_list[0][5]!=0){
      arrayOfNeighbors[1][5][2] = plist_str -> piece_list[0][5];
    }

    //A7
    if(plist_str -> piece_list[0][7]!=0){
      arrayOfNeighbors[1][7][2] = plist_str -> piece_list[0][7];
    }

    //C1
    if(plist_str -> piece_list[2][1]!=0){
      arrayOfNeighbors[1][1][3] = plist_str -> piece_list[2][1];
    }

    //C3
    if(plist_str -> piece_list[2][3]!=0){
      arrayOfNeighbors[1][1][3] = plist_str -> piece_list[2][3];
    }

    //C5
    if(plist_str -> piece_list[2][5]!=0){
      arrayOfNeighbors[1][1][3] = plist_str -> piece_list[2][5];
    }

    //C7
    if(plist_str -> piece_list[2][7]!=0){
      arrayOfNeighbors[1][1][3] = plist_str -> piece_list[2][7];
    }

    //B1
    if(plist_str -> piece_list[1][1]!=0){
      arrayOfNeighbors[0][1][3] = plist_str -> piece_list[2][7];
      arrayOfNeighbors[2][1][2] = plist_str -> piece_list[2][7];
    }

    //B3
    if(plist_str -> piece_list[1][3]!=0){
      arrayOfNeighbors[0][3][3] = plist_str -> piece_list[1][3];
      arrayOfNeighbors[2][3][2] = plist_str -> piece_list[1][3];
    }


    //B5
    if(plist_str -> piece_list[1][5]!=0){
      arrayOfNeighbors[0][5][3] = plist_str -> piece_list[1][5];
      arrayOfNeighbors[2][5][2] = plist_str -> piece_list[1][5];
    }

    //B7
    if(plist_str -> piece_list[1][7]!=0){
      arrayOfNeighbors[0][7][3] = plist_str -> piece_list[1][7];
      arrayOfNeighbors[2][7][2] = plist_str -> piece_list[1][7];
    }
    /*
    //Ausgabe
    for(int x = 0; x<3; x++){
      for(int y = 0; y<8; y++){
        for(int z = 0; z<4; z++){
          printf("%i %i %i: %i \n",x,y,z,arrayOfNeighbors[x][y][z]);
        }
      }
    }
    */
    //0.Mühlenfall: Wenn eine Mühle vorhanden ist, müssen gegnerische Steine geschlagen werden
    if(plist_str -> piecesToRemove>0){

      int countEnemyPieces = 0;

      for(int x = 0; x<3; x++){
        for(int y = 0; y<8; y++){
          if(plist_str -> piece_list[x][y] == 2){
            countEnemyPieces++;
          }
        }
      }

      randomnumber = (rand() % countEnemyPieces)+1;

      countEnemyPieces = 0;

      for(int x = 0; x<3; x++){
        for(int y = 0; y<8; y++){
          if(plist_str -> piece_list[x][y] == 0){
            countEnemyPieces++;
            if(countEnemyPieces == randomnumber){

              strcpy(answer,convertPositionToString(x,y));
              //printf("%s\n",answer);
            }
          }
        }
      }

    }

    //1. Setzphase
    if(plist_str -> unplacedPieces > 0 && plist_str -> piecesToRemove == 0){

    int countFreeSpaces = 0;
   

    for(int x = 0; x<3; x++){
      for(int y = 0; y<8; y++){
        if(plist_str -> piece_list[x][y] == 0){
          countFreeSpaces++;
        }
      }
    }

    randomnumber = (rand() % countFreeSpaces)+1;
    
    countFreeSpaces = 0;
 
    for(int x = 0; x<3; x++){
      for(int y = 0; y<8; y++){
        if(plist_str -> piece_list[x][y] == 0){
          countFreeSpaces++;
          if(countFreeSpaces == randomnumber){

            strcpy(answer,convertPositionToString(x,y));
            printf("%s\n",answer);
          }
        }
      }
    }

    }

    //2. Schieb-Phase

    if(plist_str -> unplacedPieces == 0 && plist_str -> countMyPieces > 3 && plist_str -> piecesToRemove == 0){


    }

    // TODO
    /**
     * Methode die die derzeitige Phase bestimmt
     *
     * Valider Zug - Methode (jeweils für die Phasen)
     *
     * Gegner-Such - Methode
     *
     * Setz-Phase - Methode
     *
     * Schieb-Phase - Methode
     *
     * Spring-Phase - Methode
     *
     */


    set_think_flag(false,shm_str);

    if(!write_to_pipe(fd, answer)){end_routine(shm_str, plist_str, shm_id, plist_id);}
}

