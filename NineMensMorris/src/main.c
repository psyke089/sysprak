#include "main.h"
#include "thinker/thinker.h"
#include "config.h"
#include "logger/logger.h"     //#include "shm/shmManager.h"

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
              logPrnt('r', 'e', "\nZu wenig Argumente...\n");
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
      logPrnt('r', 'e', "\nDie Länge der Game-ID muss 11 Zeichen lang sein!\n");
      printHowToUse();
  }
  if (idFlag == NULL){
      logPrnt('r', 'e', "\nDie ID wurde nicht erfolgreich gesetzt!\n");
      printHowToUse();
  }

  configInc = readConfig(file);

  configParamValid(configInc);

  return configInc;
}


/**
 * Teilt den laufenden Prozess in
 *
 * den Kindprozess   = Connector
 * und Elternprozess = Thinker
 *
 * globale pipe mit getter, damit think()
 * ohne Argumente aufgerufen werden kann
 */

int fd[2];

int* get_pipe(){
   return fd;
}


void forkingAction(){

  // pipes
  pipe(fd);


  // shm
  int shm_id = create_shm(SHMSZ);
  shm_struct* shm_str = attach_shm(shm_id);;

  int plist_id = create_shm(PLISTSZ);
  plist_struct* plist_str = attach_plist(plist_id);

  clear_shm(shm_str);
  clear_plist(plist_str);

  // signale
  init_sig_action();

  int pid = fork();

  switch (pid){
    case -1:
      logPrnt('r', 'e', "Failed to fork in main.c: ");
      exit(EXIT_FAILURE);
    break;

    case 0:  //Kind =^ sendet || starte Connection + Parser hier

      // pipes
      close(fd[WRITE]);


      // shm
      fill_shm_struct(shm_str);

      // example fill
      plist_str -> count = 2;



      //parser
      set_think_flag(true, shm_str);
      start_thinking();
      read_from_pipe(get_pipe());


      // end routine for child
      kill(getppid(), SIGKILL);
      end_routine();

    break;

    case 1 ... INT_MAX: // Eltern =^ empfängt Daten || starte Thinker hier

      close(fd[READ]);

      while(true){}

    break;

    default :
      logPrnt('r', 'e', "pid lower than -1 : This case should never happen!\n");
      end_routine();
    break;

  }
}

int main(int argc, char *argv[]) {

  initLog();

  configData conf_str = parseArgs(argc, argv);

  setLogLevel(conf_str.loglevel);

  forkingAction();

  return 0;
}




