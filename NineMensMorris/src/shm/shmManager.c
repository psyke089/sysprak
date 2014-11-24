#include "shmManager.h"

shm_struct *global_shm_str;

plist_struct *global_plist_str;

int global_shm_id;

int global_plist_id;



void fail_routine(){
  
  detach_shm(global_shm_str);
  detach_plist(global_plist_str);

  delete_by_shmid(global_shm_id);
  delete_by_shmid(global_plist_id);

  exit(EXIT_FAILURE);
}


int create_shm(size_t size){

  int shm_id;

  if ((shm_id = shmget(IPC_PRIVATE, size, IPC_CREAT | 0666)) < 0) {
    perror(RED "Could not create a shared memory" RESET);
    fail_routine();
  }


  switch (size) {
      case SHMSZ:
            global_shm_id = shm_id;
      break;

      case PLISTSZ:
            global_plist_id = shm_id;
      break;

      default: perror (RED "Could not assign global shm_ids! \n" RESET);
  }

  return shm_id;
}

int locate_shm(){

  int shm_id;
  key_t key = 0; 
  
  if ((shm_id = shmget(key, SHMSZ, 0666)) < 0) {
    perror(RED "Could not locate shared memory segment" RESET);
    fail_routine();
  }
  
  return shm_id;

}


shm_struct* attach_shm(int shm_id){
   
   shm_struct *shm_str; 

   if ((shm_str = shmat(shm_id, NULL, 0)) == (shm_struct *) -1) {
    perror(RED "Attaching the shared memory segment failed" RESET); 
    fail_routine();
  }

  global_shm_str = shm_str;

  return shm_str;
}


plist_struct* attach_plist(int plist_id){

   plist_struct *plist_str; 

   if ((plist_str = shmat(plist_id, NULL, 0)) == (plist_struct *) -1) {
    perror(RED "Attaching the piecelist shared memory segment failed" RESET); 
    fail_routine();
   }

   global_plist_str = plist_str;

   return plist_str;

}


void clear_shm(shm_struct *shm_s){
    memset(shm_s, 0, SHMSZ);
}


void clear_plist(plist_struct *plist_s){
    memset(plist_s, 0, PLISTSZ);
}

void detach_shm (shm_struct *shm_s){   
   if(shmdt(shm_s) != 0){
     perror (RED "Could not detach memory segment\n" RESET);
     exit(EXIT_FAILURE);
   }
}

void detach_plist (plist_struct *plist_s){   
   if(shmdt(plist_s) != 0){
     perror (RED "Could not detach plist memory segment\n" RESET);
     exit(EXIT_FAILURE);
   }
}   

void delete_by_shmid(int shm_id){

   if(shmctl(shm_id, IPC_RMID, NULL) == -1) {
     perror(RED "Could not destroy memory segment\n" RESET);
     exit(EXIT_FAILURE);
   }
}

void set_think_flag(bool to_set, shm_struct* shm_str){
  
  shm_str -> think = to_set;

}

void read_shm_struct(shm_struct* shm_str){

   if (shm_str -> p_pid == 0 || shm_str -> c_pid == 0 || (strcmp(shm_str -> gameID, "") == 0)){
      perror(RED "\nCouldn't read from shm_str pointer because the data is \n"
                 " corrupted (pids are 0 / gameID is empty)\n" RESET);
      fail_routine();
   } 

   if (shm_str -> think){
        printf (BLUE "\nParent recieved =>> \n"
                     "gameName    = %s\n"
                     "gameID      = %s\n"
                     "playerCount = %i\n"
                     "childPID    = %i\n"
                     "parentPID   = %i\n"
                     "think       = %i\n",
                      shm_str -> gameName,
                      shm_str -> gameID,
                      shm_str -> playerCount,
                      shm_str -> c_pid,
                      shm_str -> p_pid,
                      shm_str -> think);

       for (int i = 0; i < shm_str -> playerCount; i++){
   
        printf (BLUE "\nPlayer %i =>> \n"
                BLUE "playerID    = %i\n"
                BLUE "playerName  = %s\n"
                BLUE "isReady     = %i\n"
                BLUE "isLoggedIn  = %i\n",
                    (i+1),
                    shm_str -> p_structs[i].playerID,
                    shm_str -> p_structs[i].playerName,
                    shm_str -> p_structs[i].isReady,
                    shm_str -> p_structs[i].isLoggedIn);
        }
    }
    else {
      perror (RED "Sorry, think-flag is not set!" RESET);
      fail_routine();
    }

}


void fill_shm_struct(shm_struct* shm_str){

        player_struct player1;
        player1.playerID   = 15; 
        if(strcpy(player1.playerName, "Dummy Nr.1") == NULL){
          perror(RED "Couldn't copy playerName 1 in fill_shm_struct @ shmManager" RESET);
          fail_routine();
        }
        player1.isReady    = true;
        player1.isLoggedIn = true;

        player_struct player2;
        player2.playerID   = 20; 
        if(strcpy(player2.playerName, "Dummy Nr.2") == NULL){
          perror(RED "Couldn't copy playerName 2 in fill_shm_struct @ shmManager" RESET);
          fail_routine();
        }
        player2.isReady    = false;
        player2.isLoggedIn = true;

        if(strcpy(shm_str -> gameName, "Testgame Nr.1") == NULL){
          perror(RED "Couldn't copy gameName in fill_shm_struct @ shmManager" RESET);
          fail_routine();
        }
        if(strcpy(shm_str -> gameID, "E345Tg&afsd") == NULL){
          perror(RED "Couldn't copy gameID in fill_shm_struct @ shmManager" RESET);
          fail_routine();
        }
        shm_str -> playerCount  = 2;
        shm_str -> p_structs[0] = player1; 
        shm_str -> p_structs[1] = player2;
        shm_str -> c_pid        = getpid();
        shm_str -> p_pid        = getppid();
        shm_str -> think        = false;

        printf(GREEN "\nChild filled the shm_struct!\n" RESET);

}