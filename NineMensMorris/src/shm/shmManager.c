#include "shmManager.h"

key_t key = 2468951753;

int create_shm(){

  int shm_id;

  if ((shm_id = shmget(key, SHMSZ, IPC_CREAT | 0666)) < 0) {
    perror("Could not create a shared memory @ shmManager.c");
    exit(0);
  }

  return shm_id;
}

int locate_shm(){

  int shm_id;
  sleep(1);
  if ((shm_id = shmget(key, SHMSZ, 0666)) < 0) {
    perror("Could not locate shared memory segment @ shmManager.c");
    exit(0);
  }
  
  return shm_id;

}


shm_struct* attach_shm(int shm_id){
   
   shm_struct *shm_s; 

   if ((shm_s = shmat(shm_id, NULL, 0)) == (shm_struct *) -1) {
    perror("Attaching the shared memory segment failed @ shmManager.c");
    exit(0);
  }

   return shm_s;
}

void clear_shm(shm_struct *shm_s){
    memset(shm_s, 0, SHMSZ);
}


void detach_shm (shm_struct *shm_s){   
   if(shmdt(shm_s) != 0){
     perror ("Could not detach memory segment @ shmManager\n");
     exit(0);
   }
}   


void delete_shm(int shm_id){

   if(shmctl(shm_id, IPC_RMID, NULL) == -1) {
     perror("Could not destroy memory segment @ shmManager.c \n");
     exit(0);
   }
}


void read_shm_struct(shm_struct* shm_str){

  printf (BLUE "\nRecieved =>> \n"
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
        strcpy(player1.playerName, "Dummy Nr.1");
        player1.isReady    = true;
        player1.isLoggedIn = true;

        player_struct player2;
        player2.playerID   = 20; 
        strcpy(player2.playerName, "Dummy Nr.2");
        player2.isReady    = false;
        player2.isLoggedIn = true;

        strcpy(shm_str -> gameName, "Testgame Nr.1");
        strcpy(shm_str -> gameID, "E345Tg&afsd");
        shm_str -> playerCount  = 2;
        shm_str -> p_structs[0] = player1; 
        shm_str -> p_structs[1] = player2;
        shm_str -> c_pid        = getpid();
        shm_str -> p_pid        = getppid();

}