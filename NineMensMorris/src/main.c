#include "main.h"
#include "shm/shmManager.h"
#include "config.h"

void printHowToUse (){
  printf("\n"
         "NAME:                                              \n"
         "           client - Unser 'Nine Mens Morris' client\n"
         "OPTIONS:                                           \n"
         "          -i Game-ID (11 Zeichen lang)             \n"
         "          -c Configdateipfad (optional)            \n"
         "\n");
  exit(0);
}


/* parseArgs schaut ob die übergeben Parameter in der Kommandozeile valide sind
 * ansonsten terminiert das Programm
 *
 * -i ist die Game-ID und muss genau 11 Zeichen lang sein (wird in idFlag abgespeichert)
 * -c ist für die optionale Configdatei                     (wird in path abgespeichert)
 */
configData parseArgs(int argc, char *argv[]){


char *idFlag = NULL;
FILE* file;
char path[PATHLEN];
configData configInc;

  if (argc <= 1){
    printf("\nZu wenig Argumente...\n");
    printHowToUse();
  }
  else {

     int pArg;
     file = fopen("../client.conf", "r");
     
     while ((pArg=getopt(argc, argv, "i:c:")) != -1) {
         switch (pArg) {
             case 'i':
                idFlag = optarg;
                break;
             case 'c':
                strcpy(path , "../");
                if(strstr(optarg, ".conf")) {
                  strcat(path,optarg);
                } 
                else{
                  printf("\nDateiname enthält nicht .conf!\n");
                }
                file = fopen(path, "r"); 
                break;
          }
     }  
  
      if (idFlag != NULL && strlen(idFlag) != 11){
          printf("\nDie Länge der Game-ID muss 11 Zeichen lang sein!\n");
          printHowToUse();
      }
      if (idFlag == NULL){
          printf("\nDie ID wurde nicht erfolgreich gesetzt!\n");
          printHowToUse();
      }

      configInc = readConfig(file);

      if (configInc.hostname == NULL || configInc.portnummer == 0 || configInc.artdesspiels == NULL || configInc.loglevel < 0 || configInc.loglevel > 3){
          printf("\nDie Parameter in der .conf Datei sind nicht alle angegeben!\n");
          exit(0);
      }

      if (idFlag == NULL || configInc.hostname == NULL || configInc.portnummer == 0 || configInc.artdesspiels == NULL ){
          printHowToUse();
      }
      
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
 
 int pid = fork();

 int shm_id_parent;
 shm_struct* shm_str_parent;

 int shm_id_child;
 shm_struct* shm_str_child;


  switch (pid){
      case -1:
        perror(RED "Failed to fork in main.c @ forkingAction: " RESET);
        exit(0);
      break;

      case 0:  //Kind =^ sendet || starte Connection + Parser hier

        shm_id_child = locate_shm();
        shm_str_child = attach_shm(shm_id_child);

        printf(GREEN "Child attached shm with id = %i!\n" RESET, shm_id_child);

        clear_shm(shm_str_child);

        fill_shm_struct(shm_str_child);

        printf(GREEN "Child filled the shm_struct!" RESET);

        detach_shm(shm_str_child);

        exit(0);

      break;

      default: // Eltern =^ empfängt Daten || starte Thinker hier

        shm_id_parent = create_shm();
        shm_str_parent = attach_shm(shm_id_parent);

        printf(BLUE "Parent created shm with id = %i!\n" RESET, shm_id_parent);
        
        clear_shm(shm_str_parent);

        waitpid(pid, NULL, 0);

        read_shm_struct(shm_str_parent);
        
        detach_shm(shm_str_parent);
        delete_shm(shm_id_parent);

        exit(0);

      break;

  } 
}



int main(int argc, char *argv[]) { 

  //configData config;

  parseArgs(argc, argv);

 // forkingAction();

  return 0;
}




