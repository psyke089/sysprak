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


void calc_turn(shm_struct *shm_str, plist_struct *plist_str, int shm_id, int plist_id, int *fd){
  
    if (!check_think_flag(shm_str)){end_routine(shm_str, plist_str, shm_id, plist_id);}

    char *answer = malloc (ANSWERLENGTH * sizeof(char*));

    answer = "A1";
    
    set_think_flag(false,shm_str);

    memset(plist_str -> piece_list, 0, 3*8*sizeof(int));
    
    if(!write_to_pipe(fd, answer)){end_routine(shm_str, plist_str, shm_id, plist_id);}
    free(answer);

}

