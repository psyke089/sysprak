#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <fcntl.h>
#include <unistd.h>
#include <getopt.h>
#include <string.h>
/*#include "main.h"*/

#define MSGLEN 100
#define PATHLEN 100

typedef struct {
  char hostname[MSGLEN];
  int portnummer;
  char artdesspiels[MSGLEN];
  int loglevel;
}configData;


/**
 * wenn file == NULL 
 * öffne common_config
 *
 * ist gespeichert in "NineMansMorris/"
 * oder               "NineMansMorris/bin" 
 *
 * Terminiert bei Fehlversuch
 */
FILE* openCommonConfig(FILE *file);


/**
 * Versucht den Pfad zu öffnen
 *
 * gibt bei Fehlversuch NULL zurück  
 */
FILE* openPathConfig(char *optarg);


/**
 * Überprüft die Validität der ausgelesenen
 * Config Datei
 *
 * Terminiert bei Fehlversuch
 */
void configParamValid(configData conf_str);

/**
 * Zum Testen
 * 
 */

void printConfigString(configData conf_str);

/**
 * Parser für den .conf-File 
 *
 * schließt den Filedescriptor am Ende
 */
configData readConfig(FILE* file);

/**
 *
 * Terminiert immer
 */
void printHowToUse ();