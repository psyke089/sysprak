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


/* parseServerMsg parsed die Nachricht die vom Server kommt
 * sie benutzt das Leerzeichen als delimiter und druckt jedes Wort aus
 *
 * aktuelle maximale Wortlänge sind 64 Zeichen
 * Abspeichern in einem großzügig angelegten 2D Array um mit Sicherheit keine Wörter zu verlieren
 * TODO ^ || gerade wird die Nachricht einfach in die Konsole ausgedruckt
 * Wenn performConnection fertig ist, muss man diese Funktion noch dahin verlegen
 */
void parseServerMsg(char *msg){
  char word [64];
  int wordLength = 0;
  int wordCount = 0;

   while ( sscanf(msg, "%63[^ ]%n", word, &wordLength) == 1 ){
      wordCount++;
      printf("parsedMsg Nr.%i = \"%s\"\n", wordCount, word);
      msg += wordLength; 
           if ( *msg != ' ' ){
                break;
           }
      ++msg;
    }
}

int main(int argc, char *argv[]) { 

  parseArgs(argc, argv);

  // DEBUG START
  printf("\nTesting parseServerMsg with \"+ MNM Gameserver\" \n");
  parseServerMsg("+ MNM Gameserver");
  // DEBUG END
  return 0;
}




