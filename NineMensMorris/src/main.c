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
FILE* file = NULL;
char path[PATHLEN];
configData configInc;
int pArg;

  if (argc <= 1){
    printf("\nZu wenig Argumente...\n");
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
                     printf (RED "Couldn't copy the c-Flag to path\n" RESET);
                  }

                  if ((file = fopen(path, "r")) == NULL){
                     printf (RED "Couldn't open %s\n" RESET, path);
                  }
                  else {
                     printf (GREEN "Using %s!\n" RESET, path);
                  }

                break;
          }
     }

       if (file == NULL){ 
         printf(GREEN "Using common client.conf\n" RESET);
         if ((file = fopen("client.conf", "r")) == NULL){
          perror ("Couldn't open client.conf");
          exit(0);
         }
       }

      if (idFlag != NULL && strlen(idFlag) != 11){
          printf(RED "Die Länge der Game-ID muss 11 Zeichen lang sein!\n" RESET);
          printHowToUse();
      }
      if (idFlag == NULL){
          printf(RED "\nDie ID wurde nicht erfolgreich gesetzt!\n" RESET);
          printHowToUse();
      }

      configInc = readConfig(file);

// ############# Testing output ##############
      printf ("host = %s \n"
              "port = %i \n"
              "artds= %s  \n"
              "loglvl = %i", 
              configInc.hostname,
              configInc.portnummer,
              configInc.artdesspiels,
              configInc.loglevel);
// ################# END ####################


      if (configInc.hostname == NULL || configInc.portnummer == 0 || configInc.artdesspiels == NULL || configInc.loglevel < 0 || configInc.loglevel > 3){
          printf(RED "\nDie Parameter in der .conf Datei sind nicht alle richtig angegeben!\n" RESET);
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




