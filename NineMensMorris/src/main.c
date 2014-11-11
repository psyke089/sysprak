#include "main.h"

char *id = NULL, *to = NULL;


void printHowToUse (){
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
 * -i ist die Game-ID und muss genau 11 Zeichen lang sein
 *    pauschal auf 12345678900
 * -a ist ein Platzhalter
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
                id = optarg; break;
             case 'a':
                to = optarg; break;
             default:
                printHowToUse(); break;
          }
     }  
  
      if (id != NULL && strlen(id) != 11){
          printf("Die Länge der Game-ID muss 11 Zeichen lang sein!\n");
          exit(0);
      }
      if (id == NULL){
          printf("\nDie ID wurde nicht erfolgreich gesetzt!\n");
          printHowToUse();
          exit(0);
      }
  }
}



int main(int argc, char *argv[]) { 
  parseArgs(argc, argv);


  char *help[3];
  help [0] = "bin/performConnection";
  help [1] = id;
  help [2] = NULL;



  execv("bin/performConnection", help);
  return 0;
}




