#include "main.h"
#include "shm/shmManager.h"
#include "thinker/thinker.h"
#include "config.h"

void printHowToUse (){
  printf("\n"
         "NAME:                                              \n"
         "           client - Unser 'Nine Mens Morris' client\n"
         "OPTIONS:                                           \n"
         "          -i Game-ID (11 Zeichen lang)             \n"
         "          -c Relativer configdateipfad (optional)  \n"
         "\n");
  exit(EXIT_FAILURE);
}


/* parseArgs schaut ob die übergeben Parameter in der Kommandozeile valide sind
 * ansonsten terminiert das Programm
 *
 * -i ist die Game-ID und muss genau 11 Zeichen lang sein (wird in idFlag abgespeichert)
 * -c ist für die optionale Configdatei                     (wird in path abgespeichert)
 */
configData parseArgs(int argc, char *argv[]){


char *idFlag = NULL;
FILE* file = NULL;
char path[PATHLEN];
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
             if (strcpy(path,optarg) == NULL){
               perror (RED "\nCouldn't copy the c-Flag to path\n" RESET);
             }
             if ((path[strlen(path) - 1] != 'f') || ((file = fopen(path, "r")) == NULL) ){
                   printf (RED "\nPath: %s\n", path);
                   perror ("Couldn't open" RESET);
               } 
               else{printf (GREEN "\nUsing %s\n" RESET, path);}
           break;
      }
    }

    if (file == NULL){ 
      if ((file = fopen("client.conf", "r")) == NULL){
        if ((file = fopen("../client.conf", "r")) == NULL){
            perror ("Couldn't open client.conf");
            exit(EXIT_FAILURE);
        } else {printf(GREEN "\nUsing common client.conf\n" RESET);}
      } else {printf(GREEN "\nUsing common client.conf\n" RESET);}
    }
  
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

      fclose(file);

// ############# Testing output ##############
      printf ("host = %s \n"
              "port = %i \n"
              "artds= %s \n"
              "loglvl = %i \n", 
              configInc.hostname,
              configInc.portnummer,
              configInc.artdesspiels,
              configInc.loglevel);
// ################# END ####################


      if (configInc.hostname == NULL || configInc.portnummer == 0 || configInc.artdesspiels == NULL || configInc.loglevel < 0 || configInc.loglevel > 3){
          perror(RED "\nDie Parameter in der .conf Datei sind nicht alle richtig angegeben!\n" RESET);
          exit(EXIT_FAILURE);
      }

      if (idFlag == NULL || configInc.hostname == NULL || configInc.portnummer == 0 || configInc.artdesspiels == NULL ){
          printHowToUse();
      }
   
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
int fd[1];
pipe(fd);
char *sampleTurn = "A1";


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
        set_think_flag(shm_str);

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

        write_to_pipe(fd, sampleTurn);
        

        // Fehler bei der Übergabe von hier definierten strings
      //  write_to_pipe(fd, "He");
          
        detach_shm(shm_str);
        detach_plist(plist_str);

        delete_by_shmid(shm_id);
        delete_by_shmid(plist_id);

        exit(EXIT_SUCCESS);

      break;

      default :
        perror (RED "pid lower than -1 : This case should never happen!\n" RESET);
        exit(EXIT_FAILURE);
      break;

  } 
}



int main(int argc, char *argv[]) { 

  parseArgs(argc, argv);

  forkingAction();

  return 0;
}




