#include "main.h"
#include "shm/shmManager.h"


char *idFlag = NULL, *configFlag = NULL;

void printHowToUse (){
  printf("\n"
         "NAME:                                              \n"
         "           client - Unser 'Nine Mens Morris' client\n"
         "OPTIONS:                                           \n"
         "          -i Game-ID (11 Zeichen lang)             \n"
         "          -c Configdatei (optional)                \n"
         "\n");
  exit(0);
}


/* parseArgs schaut ob die übergeben Parameter in der Kommandozeile valide sind
 * ansonsten terminiert das Programm
 *
 * -i ist die Game-ID und muss genau 11 Zeichen lang sein (wird in idFlag abgespeichert)
 * -c ist für die optionale Config Datei              (wird in configFlag abgespeichert)
 */
void parseArgs(int argc, char *argv[]){

  if (argc <= 1){
    printf("\nZu wenig Argumente...\n");
    exit(0);
  }
  else {

     int pArg; 
     while ((pArg=getopt(argc, argv, "i:c:")) != -1) {
         switch (pArg) {
             case 'i':
                idFlag = optarg; break;
             case 'c':
                configFlag = optarg; break;
             default:
                printHowToUse(); break;
          }
     }  
  
      if (idFlag != NULL && strlen(idFlag) != 11){
          printf("Die Länge der Game-ID muss 11 Zeichen lang sein!\n");
          exit(0);
      }
      if (idFlag == NULL){
          printf("\nDie ID wurde nicht erfolgreich gesetzt!\n");
          printHowToUse();
          exit(0);
      }
  }
}



/**
 * forks into two processes
 *
 * child = Connector
 * parent = Thinker
 */

void forkingAction(){
  int pid = fork();
  int shm_id_parent;
  shm_struct* shm_s_parent;
 
 int shm_id_child;
 
 shm_struct* shm_s_child;


  switch (pid){
      case -1:
        perror(RED "Failed to fork in main.c @ forkingAction: " RESET);
        exit(0);
      break;

      case 0:  //child =^ sends 
        // start Connection here
        shm_id_child = locate_shm();

        shm_s_child = attach_shm(shm_id_child);

        clear_shm(shm_s_child);

        // do something


      break;

      default: // parent =^ recieves
        // start Thinker here

        
        shm_id_parent = create_shm();

        shm_s_parent = attach_shm(shm_id_parent);

        clear_shm(shm_s_parent);

        // do something


        waitpid(pid, NULL, 0);
        exit(0);
      break;

  } 
}



int main(int argc, char *argv[]) { 

  parseArgs(argc, argv);

  return 0;
}




