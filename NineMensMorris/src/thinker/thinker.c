#include "thinker.h"
#include "../config.h"
#include "../logger/logger.h"
//#include "../shm/shmManager.h"

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
    
    // FÜR LINUX AUSKOMMENTIEREN 
    //  asprintf(&log_msg, "\nRead %i bytes from the pipe: %s\n", bytes, buffer);
    //##########################################

    // FÜR MAC AUSKOMMENTIEREN
    char log_msg[50];
    sprintf(log_msg, "\nRead %i bytes from pipe!\n", bytes);
    //##########################################

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

    char *answer = malloc (ANSWERLENGTH * sizeof(char*));

    srand(time(NULL));

    int randomnumber;

    //0 Links (y-1) im Ring
    //1 Recht (y+1) im Ring
    //2 Unten (x+1) im Ring
    //3 Oben  (x-1) im Ring
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

    //Brückenfälle

    //Setze alle arrayOfNeighbors oben und unten auf 3
    for(int x = 0; x<3; x++){
      for(int y = 0; y<8; y++){
      arrayOfNeighbors[x][y][2] = 3;
      arrayOfNeighbors[x][y][3] = 3;
      }
    }

    //A1
    if(plist_str -> piece_list[0][1]!=0){
      arrayOfNeighbors[1][1][2] = plist_str -> piece_list[0][1];
    }else{
      arrayOfNeighbors[1][1][2] = 0;
    }

    //A3
    if(plist_str -> piece_list[0][3]!=0){
      arrayOfNeighbors[1][3][2] = plist_str -> piece_list[0][3];
    }else{
      arrayOfNeighbors[1][3][2] = 0;
    }

    //A5
    if(plist_str -> piece_list[0][5]!=0){
      arrayOfNeighbors[1][5][2] = plist_str -> piece_list[0][5];
    }else{
      arrayOfNeighbors[1][5][2] = 0;
    }

    //A7
    if(plist_str -> piece_list[0][7]!=0){
      arrayOfNeighbors[1][7][2] = plist_str -> piece_list[0][7];
    }else{
      arrayOfNeighbors[1][7][2] = 0;
    }

    //C1
    if(plist_str -> piece_list[2][1]!=0){
      arrayOfNeighbors[1][1][3] = plist_str -> piece_list[2][1];
    }else{
      arrayOfNeighbors[1][1][3] = 0;
    }

    //C3
    if(plist_str -> piece_list[2][3]!=0){
      arrayOfNeighbors[1][3][3] = plist_str -> piece_list[2][3];
    }else{
      arrayOfNeighbors[1][3][3] = 0;
    }

    //C5
    if(plist_str -> piece_list[2][5]!=0){
      arrayOfNeighbors[1][5][3] = plist_str -> piece_list[2][5];
    }else{
      arrayOfNeighbors[1][5][3] = 0;
    }


    //C7
    if(plist_str -> piece_list[2][7]!=0){
      arrayOfNeighbors[1][7][3] = plist_str -> piece_list[2][7];
    }else{
      arrayOfNeighbors[1][7][3] = 0;
    }


    //B1
    if(plist_str -> piece_list[1][1]!=0){
      arrayOfNeighbors[0][1][2] = plist_str -> piece_list[1][1];
      arrayOfNeighbors[2][1][3] = plist_str -> piece_list[1][1];
    }else{
      arrayOfNeighbors[0][1][2] = 0;
      arrayOfNeighbors[2][1][3] = 0;
    }

    //B3
    if(plist_str -> piece_list[1][3]!=0){
      arrayOfNeighbors[0][3][2] = plist_str -> piece_list[1][3];
      arrayOfNeighbors[2][3][3] = plist_str -> piece_list[1][3];
    }else{
      arrayOfNeighbors[0][3][2] = 0;
      arrayOfNeighbors[2][3][3] = 0;
    }

    //B5
    if(plist_str -> piece_list[1][5]!=0){
      arrayOfNeighbors[0][5][2] = plist_str -> piece_list[1][5];
      arrayOfNeighbors[2][5][3] = plist_str -> piece_list[1][5];
    }else{
      arrayOfNeighbors[0][5][2] = 0;
      arrayOfNeighbors[2][5][3] = 0;
    }

    //B7
    if(plist_str -> piece_list[1][7]!=0){
      arrayOfNeighbors[0][7][2] = plist_str -> piece_list[1][7];
      arrayOfNeighbors[2][7][3] = plist_str -> piece_list[1][7];
    }else{
      arrayOfNeighbors[0][7][2] = 0;
      arrayOfNeighbors[2][7][3] = 0;
    }

    
    //Ausgabe
     for(int x = 0; x<3; x++){
       for(int y = 0; y<8; y++){
   //      for(int z = 0; z<4; z++){
           printf("piece_list[%i][%i]: %i \n ", x,y,plist_str -> piece_list[x][y]); // arrayOfNeighbors[%i][%i][%i]: %i \n",x,y,plist_str -> piece_list[x][y],x,y,z,arrayOfNeighbors[x][y][z]);
   //      }
       }
     }
    

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
          if(plist_str -> piece_list[x][y] == 2){
            countEnemyPieces++;
            if(countEnemyPieces == randomnumber){

              strcpy (answer, convertPositionToString(x,y));
              printf("\nMühlenfall: %s unplaced: %i\n\n\n",answer,plist_str -> unplacedPieces);
            }
          }
        }
      }
   
    }

    //1. Setzphase
    if(plist_str -> unplacedPieces > 0 && plist_str -> piecesToRemove == 0){
/*
      if(plist_str -> unplacedPieces > 6){
        if(plist_str -> unplacedPieces == 9){strcpy(answer,convertPositionToString(0,0));plist_str -> unplacedPieces--;printf("%i\n",plist_str -> unplacedPieces);}

        else if(plist_str -> unplacedPieces == 8){strcpy(answer,convertPositionToString(0,1));plist_str -> unplacedPieces--;printf("%i\n",plist_str -> unplacedPieces);} 

        else{strcpy(answer,convertPositionToString(0,2));plist_str -> unplacedPieces--;printf("%i\n",plist_str -> unplacedPieces);} 
      }else{
*/
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

              strcpy(answer, convertPositionToString(x,y));
              plist_str -> unplacedPieces--;
              printf("\nSetzphase: %s unplaced: %i\n\n\n",answer,plist_str -> unplacedPieces);
            }
          }
        }
      /*A0-A3 }*/
      }

    }

    //2. Schieb-Phase

    if(plist_str -> unplacedPieces == 0 && plist_str -> countMyPieces > 3 && plist_str -> piecesToRemove == 0){

           int countMyPiecesWithFreeNeighbor = 0;

            for(int x = 0; x<3; x++){
              for(int y = 0; y<8; y++){
                if(plist_str -> piece_list[x][y] == 1 &&
                    (arrayOfNeighbors[x][y][0] == 0 
                  || arrayOfNeighbors[x][y][1] == 0 
                  || arrayOfNeighbors[x][y][2] == 0
                  || arrayOfNeighbors[x][y][3] == 0)){
                  countMyPiecesWithFreeNeighbor++;
                }
              }
            }

            randomnumber = (rand() % countMyPiecesWithFreeNeighbor)+1;

            countMyPiecesWithFreeNeighbor = 0;

            for(int x = 0; x<3; x++){
              for(int y = 0; y<8; y++){
                if(plist_str -> piece_list[x][y] == 1 &&
                    (arrayOfNeighbors[x][y][0] == 0 
                  || arrayOfNeighbors[x][y][1] == 0 
                  || arrayOfNeighbors[x][y][2] == 0
                  || arrayOfNeighbors[x][y][3] == 0)){
                  countMyPiecesWithFreeNeighbor++;
                  if(countMyPiecesWithFreeNeighbor == randomnumber){
                    free(answer);
                    strcpy(answer,convertPositionToString(x,y));
                    strcat(answer,":");

                    if(arrayOfNeighbors[x][y][0] == 0){
                    if(y==0){y=8;}
                    strcat(answer,convertPositionToString(x,y-1));
                    }else if(arrayOfNeighbors[x][y][1] == 0){
                      if(y==7){y=-1;}
                      strcat(answer,convertPositionToString(x,y+1));
                      if(y==-1){y=7;}
                    }else if(arrayOfNeighbors[x][y][2] == 0){
                      strcat(answer,convertPositionToString(x+1,y));   
                    }else{
                      strcat(answer,convertPositionToString(x-1,y));  
                    }
                    //strcat(answer,".");
                    printf("\nSchiebphase: %s unplaced: %i\n\n\n",answer,plist_str -> unplacedPieces);

                  }
                }
              }
            }

    }


    if(plist_str -> unplacedPieces == 0 && plist_str -> countMyPieces <= 3){
     //     free(answer);
      strcpy(answer, "DONE!");
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

    plist_str -> countMyPieces = 0;
    plist_str -> countEnemyPieces = 0;
    plist_str -> piecesToRemove = 0;

    set_think_flag(false,shm_str);


            for(int i = 0; i < 3; i++){
                for(int j = 0; j < 8; j++){
                  plist_str -> piece_list[i][j] = 0;
                }
            }
    
    if(!write_to_pipe(fd, answer)){end_routine(shm_str, plist_str, shm_id, plist_id);}
}

