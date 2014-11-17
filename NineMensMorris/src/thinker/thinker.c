#include "thinker.h"

int create_shm(){

  int shm_id;

  if ((shm_id = shmget(IPC_PRIVATE, SHMSZ, IPC_CREAT | 0666)) < 0) {
    perror("Could not create a shared memory @ Thinker.c");
    exit(0);
  }

  return shm_id;
}


shm_struct* attach_shm(int shm_id){
   
   shm_struct *shm_s; 

   if ((shm_s = shmat(shm_id, NULL, 0)) == (shm_struct *) -1) {
    perror("Attaching the shared memory segment failed @ Thinker.c");
    exit(0);
  }

   return shm_s;
}

void clear_shm(shm_struct *shm_s){
    memset(shm_s, 0, SHMSZ);
}

void delete_shm(int shm_id){
   if(shmctl(shm_id, IPC_RMID, NULL) == -1){
     perror("Could not destroy memory segment @ Thinker.c \n");
     exit(0);
   }
}


int main(){
 return 1;
}