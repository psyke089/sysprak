#include "../logger/logger.h"
#include "shmManager.h"



void end_routine(shm_struct *shm_str, plist_struct *plist_str, int shm_id, int plist_id){

   if (shm_str != NULL){detach_shm(shm_str);}
   if (plist_str != NULL){detach_plist(plist_str);}


   if (shm_id > 0){delete_by_shmid(shm_id);}
   if (plist_id > 0){delete_by_shmid(plist_id);}
   
  logPrnt('g', 'p', "\nDelete routine was executed!\n");

  exit(EXIT_SUCCESS);
}


int create_shm(size_t size){

  int shm_id;

  if ((shm_id = shmget(IPC_PRIVATE, size, IPC_CREAT | 0666)) < 0) {
    logPrnt('r', 'e', "Could not create a shared memory");
  }

  return shm_id;
}


shm_struct* attach_shm(int shm_id){
   
   shm_struct *shm_str; 

   if ((shm_str = shmat(shm_id, NULL, 0)) == (shm_struct *) -1) {
    logPrnt('r', 'e', "Attaching the shared memory segment failed"); 
  }

  return shm_str;
}


plist_struct* attach_plist(int plist_id){

   plist_struct *plist_str; 

   if ((plist_str = shmat(plist_id, NULL, 0)) == (plist_struct *) -1) {
    logPrnt('r', 'e', "Attaching the piecelist shared memory segment failed"); 
   }

   return plist_str;

}


void clear_shm(shm_struct *shm_s){
    memset(shm_s, 0, SHMSZ);
}


void clear_plist(plist_struct *plist_s){
    memset(plist_s, 0, PLISTSZ);
}

int detach_shm (shm_struct *shm_s){   

   if(shmdt(shm_s) != 0){
     logPrnt('r', 'e', "Could not detach memory segment\n");
     return 0;
   }
     return 1;
}

int detach_plist (plist_struct *plist_s){   
   if(shmdt(plist_s) != 0){
     logPrnt('r', 'e', "Could not detach plist memory segment\n");
     return 0;
   }
     return 1;
}   

int delete_by_shmid(int shm_id){
   if(shmctl(shm_id, IPC_RMID, NULL) == -1) {
     logPrnt('r', 'e', "Could not delete memory segment\n");
     return 0;
   }
     return 1;
}

void set_think_flag(bool to_set, shm_struct* shm_str){
  shm_str -> think = to_set;
}

int check_think_flag(shm_struct* shm_str){
  if (shm_str -> think){
    set_think_flag(false, shm_str);
    return 1;
  }
  
    logPrnt('r', 'e', "Sorry, the think-flag has not been set");
    return 0;
}

int read_shm_struct(shm_struct* shm_str){

   if (shm_str -> p_pid == 0 || shm_str -> c_pid == 0 || (strcmp(shm_str -> gameID, "") == 0)){
      logPrnt('r', 'e', "\nCouldn't read from shm_str pointer because the data is \n"
                        " corrupted (pids are 0 / gameID is empty)\n");
      return 0;
   } 

  
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
        return 1;
}


int fill_shm_struct(shm_struct* shm_str){

        player_struct player1;
        player1.playerID   = 15; 
        if(strcpy(player1.playerName, "Dummy Nr.1") == NULL){
         logPrnt('r', 'e', "Couldn't copy playerName 1 in fill_shm_struct @ shmManager");
         return 0;
        }
        player1.isReady    = true;
        player1.isLoggedIn = true;

        player_struct player2;
        player2.playerID   = 20; 
        if(strcpy(player2.playerName, "Dummy Nr.2") == NULL){
          logPrnt('r', 'e', "Couldn't copy playerName 2 in fill_shm_struct @ shmManager");
          return 0;
        }
        player2.isReady    = false;
        player2.isLoggedIn = true;

        if(strcpy(shm_str -> gameName, "Testgame Nr.1") == NULL){
          logPrnt('r', 'e', "Couldn't copy gameName in fill_shm_struct @ shmManager");
          return 0;
        }
        if(strcpy(shm_str -> gameID, "E345Tg&afsd") == NULL){
          logPrnt('r', 'e', "Couldn't copy gameID in fill_shm_struct @ shmManager");
          return 0;
        }
        shm_str -> playerCount  = 2;
        shm_str -> p_structs[0] = player1; 
        shm_str -> p_structs[1] = player2;
        shm_str -> c_pid        = getpid();
        shm_str -> p_pid        = getppid();
        shm_str -> think        = false;

        logPrnt('g', 'p', "\nChild filled the shm_struct!\n");
          return 1;

}