#include "main.h"

char *id = NULL, *to = NULL;


void printHowToUse (){
  printf("NAME:                                              \n"
         "           client - Unser 'Nine Mens Morris' client\n"
         "OPTIONS:                                           \n"
         "          -i Game-ID                               \n"
         "          -a PLACEHOLDER                         \n\n");
  exit(0);
}


void parseArgs(int argc, char *argv[]){

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

  if (strlen(id) != 11){
    printf ("Die Länge der Game-ID muss 11 Zeichen lang sein!");
    exit(0);
  }

}



int main(int argc, char *argv[]) { 

  parseArgs(argc, argv);

  //DEBUG
  printf("Ihre (-a) to ist: %s\n",to);

  return EXIT_SUCCESS;
}




