#include "thinker.h"


char* read_from_pipe(int *fd){
 
   static char buffer[ANSWERLENGTH];
   int bytes;

   if ((bytes = read(fd[READ], buffer, ANSWERLENGTH)) <= 0) {
//        buf[n] = 0; /* terminate the string */
        perror (RED "Couldn't read from pipe!" RESET);
        memset(buffer, 0, ANSWERLENGTH);
   }
   else {
        printf (GREEN "Read %i bytes from the pipe: %s\n" RESET, bytes, buffer);
   }

   return buffer;

}


void write_to_pipe(int *fd, char *str){
 
   if (str != NULL && write(fd[WRITE], str, strlen(str)) <= 0) {
        perror (RED "Couldn't write to pipe!" RESET);
        exit(EXIT_FAILURE);
   }

}


