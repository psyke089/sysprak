#include "thinker.h"
#include "../logger/logger.h"
#include "../main.h"

void start_thinking(){
    kill (getppid(), SIGUSR1);
}


void init_sig_action(){
    struct sigaction sig_str;
    sig_str.sa_handler = think;
    sigaction (SIGUSR1, &sig_str, NULL);
}


char* read_from_pipe(int *fd){
   
   //char *log_msg = 0;
   static char buffer[ANSWERLENGTH];
   int bytes;

   if ((bytes = read(fd[READ], buffer, ANSWERLENGTH)) <= 0) {
        logPrnt('r', 'e', "\nCouldn't read from pipe!\n");
        memset(buffer, 0, ANSWERLENGTH);
        end_routine();
   }
   else {
    
    //asprintf(&log_msg, "\nRead %i bytes from the pipe: %s\n", bytes, buffer);
    //strcpy(log_msg, "\nRead some bytes from pipe!\n");
    //logPrnt('g', 'p', log_msg);
    //free(log_msg);
   }

   return buffer;

}


void write_to_pipe(int *fd, char *str){
 
   if (str == NULL || write(fd[WRITE], str, strlen(str)) <= 0) {
        logPrnt('r', 'e', "\nCouldn't write to pipe!\n");
        end_routine();
   }

   set_think_flag(false,get_shm_struct());
}


void think(){
  
    // think-flag muss überprüft werden
    check_think_flag(get_shm_struct());

    // hole die pieces liste
    plist_struct* plist_str = get_plist_struct();

    // testoutput
    printf("Der Beispielfill von plist_str -> count ist = %i\n", plist_str -> count);


    // TODO
    /**
     * Methode die die derzeitige Phase bestimmt
     *
     * Valider Zug - Methode (jeweils für die Phasen)
     *
     * Gegner-Such - Methode
     *
     * Setz-Phase - Methode
     *
     * Schieb-Phase - Methode
     *
     * Spring-Phase - Methode
     *
     */
    // berechne die antwort und speichere sie in answer

    char answer[ANSWERLENGTH] = "A1";

    write_to_pipe(get_pipe(), answer);
}





