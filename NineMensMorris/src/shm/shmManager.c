#include "shmManager.h"

int create_shm(){

  int shm_id;

  if ((shm_id = shmget(IPC_PRIVATE, SHMSZ, IPC_CREAT | 0666)) < 0) {
    perror("Could not create a shared memory @ shmManager.c");
    exit(0);
  }

  return shm_id;
}

int locate_shm(){

  int shm_id;
  
  if ((shm_id = shmget(IPC_PRIVATE, SHMSZ, 0666)) < 0) {
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

void delete_shm(int shm_id, shm_struct *shm_s){
   
   if(shmdt(shm_s) != 0){
     perror ("Could not detach memory segment @ shmManager\n");
     exit(0);
   }


   if(shmctl(shm_id, IPC_RMID, NULL) == -1) {
     perror("Could not destroy memory segment @ shmManager.c \n");
     exit(0);
   }
}