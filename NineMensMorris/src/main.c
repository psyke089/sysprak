#include "main.h"
#include "thinker/thinker.h"
#include "config.h"
#include "logger/logger.h"     
#include "shm/shmManager.h"
#include "connector/connector.h"

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

  sprintf(configInc.game_id, "%s", idFlag);

  configParamValid(configInc);

  return configInc;
}

/**
 * Teilt den laufenden Prozess in
 *
 * den Kindprozess   = Connector
 * und Elternprozess = Thinker
 *
 */
 void forkingAction(configData conf_str){

  // pipes
  int fd[2];
  pipe(fd);


  // shm
  int shm_id = create_shm(SHMSZ);
  if(shm_id < 0){end_routine(NULL, NULL, (-1), (-1));}

  shm_struct* shm_str = attach_shm(shm_id);
  if (shm_str == NULL){end_routine(NULL, NULL, shm_id, (-1));}

  int plist_id = create_shm(PLISTSZ);
  if(plist_id < 0){end_routine(shm_str, NULL, shm_id, (-1));}

  plist_struct* plist_str = attach_plist(plist_id);
  if (plist_str == NULL){end_routine(shm_str, NULL, shm_id, plist_id);}

  clear_shm(shm_str);
  clear_plist(plist_str);

  //nested functions for singals
  void think (){
      calc_turn(shm_str, plist_str, shm_id, plist_id, fd);
  }
  void sig_int_handler(){
      end_routine(shm_str, plist_str, shm_id, plist_id);
  }
  
  //signals
  struct sigaction sig_str;
  sig_str.sa_handler = think;
  sigaction (SIGUSR1, &sig_str, NULL);

  int sock;
  int pid = fork();

  switch (pid){
      case -1:
        logPrnt('r', 'e', "Failed to fork in main.c: ");
        end_routine(shm_str, plist_str, shm_id, plist_id);
      break;

      case 0:  //Kind =^ sendet || starte Connection + Parser hier

        // pipes
        close(fd[WRITE]);

        //signals
        signal (SIGINT, sig_int_handler);

        // shm
        strcpy(shm_str -> gameID, conf_str.game_id);
        plist_str -> piecesToRemove = 0;
        plist_str -> unplacedPieces = 9;
        
        //parser
        sock = performConnection();
        parseMessages(sock, shm_str, plist_str, fd);

        //end routine
        close(sock);
        kill(getppid(), SIGUSR2);
        exit(EXIT_SUCCESS);
      break;

      case 1 ... INT_MAX: // Eltern =^ empfängt Daten || starte Thinker hier

        close(fd[READ]);
        signal(SIGUSR2, sig_int_handler);
        while(1){}

      break;

      default :
        logPrnt('r', 'e', "pid lower than -1 : This case should never happen!\n");
        end_routine(shm_str, plist_str, shm_id, plist_id);
      break;

  }
}

int main(int argc, char *argv[]) {

  initLog();

  configData conf_str = parseArgs(argc, argv);

  setLogLevel(conf_str.loglevel);

  forkingAction(conf_str);

  return 0;
}




