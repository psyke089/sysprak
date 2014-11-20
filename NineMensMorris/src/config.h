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
FILE* open_common_config(FILE *file);


/**
 * Versucht den Pfad zu öffnen
 *
 * gibt bei Fehlversuch NULL zurück  
 */
FILE* open_path_config(char *optarg);


/**
 * Überprüft die Validität der ausgelesenen
 * Config Datei
 *
 * Terminiert bei Fehlversuch
 */
void config_valid(configData conf_str);

/**
 * Überprüft die Validität von der ID
 *
 * Terminiert bei Fehlversuch
 */
void id_valid(char *idFlag);


/**
 * Zum Testen
 * 
 */

void print_config_str(configData conf_str);

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