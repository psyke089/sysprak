#include "main.h"

char *id = NULL, *to = NULL;


void printHowToUse (){
  printf("\n"
         "NAME:                                              \n"
         "           client - Unser 'Nine Mens Morris' client\n"
         "OPTIONS:                                           \n"
         "          -i Game-ID                               \n"
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
    id = "12345678900";
    printf("\nDie Game-ID wurde automatisch auf 12345678900 gesetzt.\n");
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
          printf ("Die Länge der Game-ID muss 11 Zeichen lang sein!\n");
          exit(0);
      }
  }
}



int main(int argc, char *argv[]) { 

  parseArgs(argc, argv);
  return 0;
}




