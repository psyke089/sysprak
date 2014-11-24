#include "main.h"
//#include "shm/shmManager.h"
#include "thinker/thinker.h"
#include "config.h"

/* parseArgs schaut ob die übergeben Parameter in der Kommandozeile valide sind
 * ansonsten terminiert das Programm
 *
 * -i ist die Game-ID und muss genau 11 Zeichen lang sein (wird in idFlag abgespeichert)
 * -c ist für die optionale Configdatei                     (wird in file abgespeichert)
 */

configData parseArgs(int argc, char *argv[]){

char *idFlag = NULL;
FILE* file = NULL;
configData configInc;
int pArg;

          if (argc <= 1){
              perror("\nZu wenig Argumente...\n");
              printHowToUse();
          }
          else {
              while ((pArg=getopt(argc, argv, "i:c:")) != -1) {
                   switch (pArg) {
                       case 'i':
                        idFlag = optarg;
                       break;

                       case 'c':
                        file = openPathConfig(optarg);
                       break;
                   }
              }
            file = openCommonConfig(file);
          }

  if (idFlag != NULL && strlen(idFlag) != 11){
      perror(RED "\nDie Länge der Game-ID muss 11 Zeichen lang sein!\n" RESET);
      printHowToUse();
  }
  if (idFlag == NULL){
      perror(RED "\nDie ID wurde nicht erfolgreich gesetzt!\n" RESET);
      printHowToUse();
  }

  configInc = readConfig(file);

  configParamValid(configInc);

  printConfigString(configInc);

  return configInc;
}

/**
 * Teilt den laufenden Prozess in 
 *
 * den Kindprozess   = Connector
 * und Elternprozess = Thinker
 */
void forkingAction(){

// pipes
int fd[2];
pipe(fd);


// shm
int shm_id = create_shm(SHMSZ);
shm_struct* shm_str = attach_shm(shm_id);;

int plist_id = create_shm(PLISTSZ);
plist_struct* plist_str = attach_plist(plist_id);

clear_shm(shm_str);
clear_plist(plist_str);

// signals
init_sig_action();

int pid = fork();

  switch (pid){
      case -1:
        perror(RED "Failed to fork in main.c: " RESET);
        exit(EXIT_FAILURE);
      break;

      case 0:  //Kind =^ sendet || starte Connection + Parser hier

        close(fd[WRITE]);

        fill_shm_struct(shm_str);
        set_think_flag(true, shm_str);



        sleep(1);
        start_thinking();

        read_from_pipe(fd);

        detach_shm(shm_str);
        detach_plist(plist_str);

        exit(EXIT_SUCCESS);

      break;

      case 1 ... INT_MAX: // Eltern =^ empfängt Daten || starte Thinker hier

        close(fd[READ]);
       
        while (!get_signal()){}

        read_shm_struct(shm_str);

        write_to_pipe(fd, think(plist_str), shm_str);
        
        fail_routine();

      break;

      default :
        perror (RED "pid lower than -1 : This case should never happen!\n" RESET);
        fail_routine();
      break;

  } 
}



int main(int argc, char *argv[]) { 

  parseArgs(argc, argv);

  forkingAction();

  return 0;
}




