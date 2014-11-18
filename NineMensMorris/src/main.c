#include "main.h"
#include "shm/shmManager.h"
#include "config.h"

char *idFlag = NULL, *configFlag = NULL;

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
 * -c ist ein Platzhalter                                 (wird in configFlag abgespeichert)
 */
configParams parseArgs(int argc, char *argv[]){

FILE* file;
char path[100];
configParams configTemp;

  if (argc <= 1){
    printf("\nZu wenig Argumente...\n");
    printHowToUse();
  }
  else {

     int pArg; 
     while ((pArg=getopt(argc, argv, "i:c:")) != -1) {
         switch (pArg) {
             case 'i':
                idFlag = optarg;
                break;
             case 'c':
                strcpy(path , "../");
                strcat(path,optarg);
                file = fopen(path, "r"); 
                break;
             default:
                file = fopen("../client.conf", "r");
                break;
          }
     }  
  
      if (idFlag != NULL && strlen(idFlag) != 11){
          printf("Die Länge der Game-ID muss 11 Zeichen lang sein!\n");
          printHowToUse();
      }
      if (idFlag == NULL){
          printf("\nDie ID wurde nicht erfolgreich gesetzt!\n");
          printHowToUse();
      }

      configTemp = readConfig(file);

      if (configTemp.hostname == NULL || configTemp.portnummer == 0|| configTemp.artdesspiels == NULL ){
          printf("\nDie Parameter in der .conf Datei sind nicht alle angegeben!\n");
          exit(0);
      }

      if (idFlag == NULL || configTemp.hostname == NULL || configTemp.portnummer == 0|| configTemp.artdesspiels == NULL ){
          printHowToUse();
      }
      
  }
  return configTemp;
}

/**
 * prettyPrint for shm_struct
 *
 **/
void read_shm_struct(shm_struct* shm_str){
  printf (BLUE "\nRecieved =>> \n"
               "gameName    = %s\n"
               "gameID      = %s\n"
               "playerCount = %i\n"
               "childPID    = %i\n"
               "parentPID   = %i\n" RESET,
           shm_str -> gameName,
           shm_str -> gameID,
           shm_str -> playerCount,
           shm_str -> c_pid,
           shm_str -> p_pid);

  for (int i = 0; i < shm_str -> playerCount; i++){

       printf (BLUE "\nPlayer %i =>> \n"
                    "playerID    = %i\n"
                    "playerName  = %s\n"
                    "isReady     = %i\n"
                    "isLoggedIn  = %i\n" RESET,
                     (i+1),
                     shm_str -> p_structs[i].playerID,
                     shm_str -> p_structs[i].playerName,
                     shm_str -> p_structs[i].isReady,
                     shm_str -> p_structs[i].isLoggedIn);
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
 shm_struct* shm_str_parent;

 int shm_id_child;
 shm_struct* shm_str_child;


  switch (pid){
      case -1:
        perror(RED "Failed to fork in main.c @ forkingAction: " RESET);
        exit(0);
      break;

      case 0:  //child =^ sends || start Connection + Parser here

        shm_id_child = locate_shm();
        shm_str_child = attach_shm(shm_id_child);
        printf(GREEN "Child attached shm with id = %i!\n" RESET, shm_id_child);

        clear_shm(shm_str_child);

        player_struct player1;
        player1.playerID   = 15; 
        strcpy(player1.playerName, "Dummy Nr.1");
        player1.isReady    = true;
        player1.isLoggedIn = true;

        player_struct player2;
        player2.playerID   = 20; 
        strcpy(player2.playerName, "Dummy Nr.2");
        player2.isReady    = false;
        player2.isLoggedIn = true;

        strcpy(shm_str_child -> gameName, "Testgame Nr.1");
        strcpy(shm_str_child -> gameID, "E345Tg&a§dx");
        shm_str_child -> playerCount  = 2;
        shm_str_child -> p_structs[0] = player1; 
        shm_str_child -> p_structs[1] = player2;
        shm_str_child -> c_pid        = getpid();
        shm_str_child -> p_pid        = getppid();
        printf(GREEN "Child filled the shm_struct!" RESET);

        detach_shm(shm_str_child);
        exit(0);

      break;

      default: // parent =^ recieves data || start Thinker here

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

  //configParams config;

  parseArgs(argc, argv);

  forkingAction();

  return 0;
}




