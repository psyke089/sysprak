#include "thinker.h"


volatile sig_atomic_t signal_set = 0;

void synch_signal (int sig){
  signal_set = 1;
}


void reset_signal (){
  signal_set = 0;
}


sig_atomic_t get_signal(){
    return signal_set;
}


void start_thinking(){
    kill (getppid (), SIGUSR1);
}


void init_sig_action(){
    struct sigaction sig_str;
    sig_str.sa_handler = synch_signal;
    sigaction (SIGUSR1, &sig_str, NULL);
}


char* read_from_pipe(int *fd){
 
   static char buffer[ANSWERLENGTH];
   int bytes;

   if ((bytes = read(fd[READ], buffer, ANSWERLENGTH)) <= 0) {
        perror (RED "Couldn't read from pipe!" RESET);
        memset(buffer, 0, ANSWERLENGTH);
   }
   else {
        printf (GREEN "\nRead %i bytes from the pipe: %s\n" RESET, bytes, buffer);
   }

   return buffer;

}


void write_to_pipe(int *fd, char *str, shm_struct *shm_str){
 
   if (str == NULL || write(fd[WRITE], str, strlen(str)) <= 0) {
        perror (RED "Couldn't write to pipe!" RESET);
        fail_routine();
   }

   reset_signal();
   set_think_flag(false, shm_str);

}


char* think(plist_struct *plist_str){
        plist_str -> piece_list[0][0] = 'A';
        plist_str -> piece_list[0][1] = '1';
    return plist_str -> piece_list[0];
}





