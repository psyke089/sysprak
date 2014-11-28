#include "shmManager.h"
#include "../logger/logger.h"

shm_struct *global_shm_str = NULL;

plist_struct *global_plist_str = NULL;

int global_shm_id = 0;

int global_plist_id = 0;


void end_routine(){

  detach_shm(global_shm_str);
  detach_plist(global_plist_str);

  delete_by_shmid(global_shm_id);
  delete_by_shmid(global_plist_id);

  logPrnt('g', 'p', "\nDelete routine was executed!\n");

  exit(EXIT_FAILURE);
}


int create_shm(size_t size){

  int shm_id;

  if ((shm_id = shmget(IPC_PRIVATE, size, IPC_CREAT | 0666)) < 0) {
    logPrnt('r', 'e', "Could not create a shared memory");
    end_routine();
  }


  switch (size) {
      case SHMSZ:
            global_shm_id = shm_id;
      break;

      case PLISTSZ:
            global_plist_id = shm_id;
      break;

      default: logPrnt('r', 'e', "Could not assign global shm_ids! \n");
  }

  return shm_id;
}

// obsolete function
/*int locate_shm(){

  int shm_id;
  key_t key = 0; 
  
  if ((shm_id = shmget(key, SHMSZ, 0666)) < 0) {
    perror(RED "Could not locate shared memory segment" RESET);
    end_routine();
  }
  
  return shm_id;

}*/


shm_struct* attach_shm(int shm_id){
   
   shm_struct *shm_str; 

   if ((shm_str = shmat(shm_id, NULL, 0)) == (shm_struct *) -1) {
    logPrnt('r', 'e', "Attaching the shared memory segment failed"); 
    end_routine();
  }

  global_shm_str = shm_str;

  return shm_str;
}


plist_struct* attach_plist(int plist_id){

   plist_struct *plist_str; 

   if ((plist_str = shmat(plist_id, NULL, 0)) == (plist_struct *) -1) {
    logPrnt('r', 'e', "Attaching the piecelist shared memory segment failed"); 
    end_routine();
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
     logPrnt('r', 'e', "Could not detach memory segment\n");
     exit(EXIT_FAILURE);
   }
}

void detach_plist (plist_struct *plist_s){   
   if(shmdt(plist_s) != 0){
     logPrnt('r', 'e', "Could not detach plist memory segment\n");
     exit(EXIT_FAILURE);
   }
}   

void delete_by_shmid(int shm_id){

   if(shmctl(shm_id, IPC_RMID, NULL) == -1) {
     logPrnt('r', 'e', "Could not destroy memory segment\n");
     exit(EXIT_FAILURE);
   }
}

void set_think_flag(bool to_set, shm_struct* shm_str){
  
  shm_str -> think = to_set;

}

void check_think_flag(shm_struct* shm_str){
    if (shm_str -> think){
      set_think_flag(false, shm_str);
    }
    else {
      logPrnt('r', 'e', "Sorry, the think-flag has not been set");
      end_routine();
    }
}

shm_struct* get_shm_struct(){
  if (global_shm_str == NULL){
    logPrnt('r', 'e', "Global shm struct is NULL");
    end_routine();
  }
  return global_shm_str;
}


plist_struct* get_plist_struct(){
  if (global_plist_str == NULL){
    logPrnt('r', 'e', "Global plist struct is NULL");
    end_routine();
  }
  return global_plist_str;
}



int get_shm_id(){
  if (global_shm_id == 0){
    logPrnt('r', 'e', "Global shm id is NULL" RESET);
    end_routine();
  }
  return global_shm_id;
}


int get_plist_id(){
  if (global_plist_id == 0){
    logPrnt('r', 'e', "Global plist id is NULL" RESET);
    end_routine();
  }
  return global_plist_id;
}

void read_shm_struct(shm_struct* shm_str){

   if (shm_str -> p_pid == 0 || shm_str -> c_pid == 0 || (strcmp(shm_str -> gameID, "") == 0)){
      logPrnt('r', 'e', "\nCouldn't read from shm_str pointer because the data is \n"
                        " corrupted (pids are 0 / gameID is empty)\n");
      end_routine();
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
}




void fill_shm_struct(shm_struct* shm_str){

        player_struct player1;
        player1.playerID   = 15; 
        if(strcpy(player1.playerName, "Dummy Nr.1") == NULL){
         logPrnt('r', 'e', "Couldn't copy playerName 1 in fill_shm_struct @ shmManager");
          end_routine();
        }
        player1.isReady    = true;
        player1.isLoggedIn = true;

        player_struct player2;
        player2.playerID   = 20; 
        if(strcpy(player2.playerName, "Dummy Nr.2") == NULL){
          logPrnt('r', 'e', "Couldn't copy playerName 2 in fill_shm_struct @ shmManager");
          end_routine();
        }
        player2.isReady    = false;
        player2.isLoggedIn = true;

        if(strcpy(shm_str -> gameName, "Testgame Nr.1") == NULL){
          logPrnt('r', 'e', "Couldn't copy gameName in fill_shm_struct @ shmManager");
          end_routine();
        }
        if(strcpy(shm_str -> gameID, "E345Tg&afsd") == NULL){
          logPrnt('r', 'e', "Couldn't copy gameID in fill_shm_struct @ shmManager");
          end_routine();
        }
        shm_str -> playerCount  = 2;
        shm_str -> p_structs[0] = player1; 
        shm_str -> p_structs[1] = player2;
        shm_str -> c_pid        = getpid();
        shm_str -> p_pid        = getppid();
        shm_str -> think        = false;

        logPrnt('g', 'p', "\nChild filled the shm_struct!\n");

}