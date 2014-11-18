#include "main.h"
#include "shm/shmManager.h"
#include "config.h"

char *idFlag = NULL, *configFlag = NULL, path[100];;
FILE* file;
char delimiter[] = " \n";
char *ptr;
size_t len = 0;
char * line = NULL;


typedef struct {
  char hostname[100];
  int portnummer;
  char artdesspiels[100];
  int loglevel;
}conf;

conf config[1];

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
 * -a ist ein Platzhalter                                 (wird in aFlag abgespeichert)
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
          exit(0);
      }
      if (idFlag == NULL){
          printf("\nDie ID wurde nicht erfolgreich gesetzt!\n");
          printHowToUse();
          exit(0);
      }


      while ( fgets(line, len, file) != NULL) {   
          ptr = strtok(line, delimiter);
          if(strcmp("hostname",ptr) == 0){
            ptr = strtok(NULL, delimiter);
            ptr = strtok(NULL, delimiter);  
            strcpy(config[0].hostname,ptr);
          }

          if(strcmp("portnummer",ptr) == 0){
            ptr = strtok(NULL, delimiter);
            ptr = strtok(NULL, delimiter);  
            config[0].portnummer = atof(ptr);
          }

          if(strcmp("artdesspiels",ptr) == 0){
            ptr = strtok(NULL, delimiter);
            ptr = strtok(NULL, delimiter);  
            strcpy(config[0].artdesspiels,ptr);
          }
      }

      printf("%s %d %s",config[0].hostname,config[0].portnummer,config[0].artdesspiels);
      //testi();

  }
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

 // parseArgs(argc, argv);
  forkingAction();
  
  return 0;
}




