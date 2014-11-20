#include "shmManager.h"

int create_shm(){

  int shm_id;

  if ((shm_id = shmget(IPC_PRIVATE, SHMSZ, IPC_CREAT | 0666)) < 0) {
    perror(RED "Could not create a shared memory @ shmManager.c" RESET);
    exit(EXIT_FAILURE);
  }

  return shm_id;
}

int locate_shm(){

  int shm_id;
  key_t key = 0; 
  
  if ((shm_id = shmget(key, SHMSZ, 0666)) < 0) {
    perror(RED "Could not locate shared memory segment @ shmManager.c" RESET);
    exit(EXIT_FAILURE);
  }
  
  return shm_id;

}


shm_struct* attach_shm(int shm_id){
   
   shm_struct *shm_s; 

   if ((shm_s = shmat(shm_id, NULL, 0)) == (shm_struct *) -1) {
    perror(RED "Attaching the shared memory segment failed @ shmManager.c" RESET); 
    exit(EXIT_FAILURE);
  }

   return shm_s;
}

void clear_shm(shm_struct *shm_s){
    memset(shm_s, 0, SHMSZ);
}


void detach_shm (shm_struct *shm_s){   
   if(shmdt(shm_s) != 0){
     perror (RED "Could not detach memory segment @ shmManager\n" RESET);
     exit(EXIT_FAILURE);
   }
}   


void delete_shm(int shm_id){

   if(shmctl(shm_id, IPC_RMID, NULL) == -1) {
     perror(RED "Could not destroy memory segment @ shmManager.c \n" RESET);
     exit(EXIT_FAILURE);
   }
}


void read_shm_struct(shm_struct* shm_str){

   if (shm_str -> p_pid == 0 || shm_str -> c_pid == 0 || (strcmp(shm_str -> gameID, "") == 0)){
      perror(RED "\nCouldn't read from shm_str pointer because the data is \n"
                 " corrupted (pids are 0 / gameID is empty) @ shmManager.c" RESET);
      exit(EXIT_FAILURE);
   } 

   printf (BLUE "\nParent recieved =>> \n"
                "gameName    = %s\n"
                "gameID      = %s\n"
                "playerCount = %i\n"
                "childPID    = %i\n"
                "parentPID   = %i\n" RESET,
                 shm_str -> gameName,
                 shm_str -> gameID,
                 shm_str -> playerCount,
                 shm_str -> c_pid,
                 shm_str -> p_pid);

   for (int i = 0; i < shm_str -> playerCount; i++){

   printf (BLUE "\nPlayer %i =>> \n"
                "playerID    = %i\n"
                "playerName  = %s\n"
                "isReady     = %i\n"
                "isLoggedIn  = %i\n" RESET,
                (i+1),
                shm_str -> p_structs[i].playerID,
                shm_str -> p_structs[i].playerName,
                shm_str -> p_structs[i].isReady,
                shm_str -> p_structs[i].isLoggedIn);
   }
}


void fill_shm_struct(shm_struct* shm_str){

        player_struct player1;
        player1.playerID   = 15; 
        if(strcpy(player1.playerName, "Dummy Nr.1") == NULL){
          perror(RED "Couldn't copy playerName 1 in fill_shm_struct @ shmManager" RESET);
          exit(EXIT_FAILURE);
        }
        player1.isReady    = true;
        player1.isLoggedIn = true;

        player_struct player2;
        player2.playerID   = 20; 
        if(strcpy(player2.playerName, "Dummy Nr.2") == NULL){
          perror(RED "Couldn't copy playerName 2 in fill_shm_struct @ shmManager" RESET);
          exit(EXIT_FAILURE);
        }
        player2.isReady    = false;
        player2.isLoggedIn = true;

        if(strcpy(shm_str -> gameName, "Testgame Nr.1") == NULL){
          perror(RED "Couldn't copy gameName in fill_shm_struct @ shmManager" RESET);
          exit(EXIT_FAILURE);
        }
        if(strcpy(shm_str -> gameID, "E345Tg&afsd") == NULL){
          perror(RED "Couldn't copy gameID in fill_shm_struct @ shmManager" RESET);
          exit(EXIT_FAILURE);
        }
        shm_str -> playerCount  = 2;
        shm_str -> p_structs[0] = player1; 
        shm_str -> p_structs[1] = player2;
        shm_str -> c_pid        = getpid();
        shm_str -> p_pid        = getppid();

}