#include "main.h"

char *idFlag = NULL, *aFlag = NULL;

void printHowaFlagUse (){
  printf("\n"
         "NAME:                                              \n"
         "           client - Unser 'Nine Mens Morris' client\n"
         "OPTIONS:                                           \n"
         "          -i Game-ID (11 Zeichen lang)             \n"
         "          -a PLACEHOLDER                           \n"
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
     while ((pArg=getopt(argc, argv, "i:a:")) != -1) {
         switch (pArg) {
             case 'i':
                idFlag = optarg; break;
             case 'a':
                aFlag = optarg; break;
             default:
                printHowaFlagUse(); break;
          }
     }  
  
      if (idFlag != NULL && strlen(idFlag) != 11){
          printf("Die Länge der Game-ID muss 11 Zeichen lang sein!\n");
          exit(0);
      }
      if (idFlag == NULL){
          printf("\nDie ID wurde nicht erfolgreich gesetzt!\n");
          printHowaFlagUse();
          exit(0);
      }
  }
}



/* creates an array aFlag pass down the arguments aFlag performConnection
 * 
 * arg [0] is always the path itself
 * arg [1..] are the arguments
 * arg [last] is always NULL
 */
void startPerformConnection(){
  char *connectionArgs[3];

    connectionArgs [0] = "bin/performConnection";
    connectionArgs [1] = idFlag;
    connectionArgs [2] = NULL;

  execv("bin/performConnection", connectionArgs);
}

int main(int argc, char *argv[]) { 

  parseArgs(argc, argv);
  startPerformConnection();

  return 0;
}




