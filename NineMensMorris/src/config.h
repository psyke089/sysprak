#ifndef config_h
#define config_h

#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <fcntl.h>
#include <unistd.h>
#include <getopt.h>
#include <string.h>

#define MSGLEN 100
#define PATHLEN 100

/**
 * Farben
 */
#define RED     "\x1b[31m"
#define GREEN   "\x1b[32m"
#define YELLOW  "\x1b[33m"
#define BLUE    "\x1b[34m"
#define MAGENTA "\x1b[35m"
#define CYAN    "\x1b[36m"
#define RESET   "\x1b[0m"


typedef struct {
  char hostname[MSGLEN];
  int portnummer;
  char artdesspiels[MSGLEN];
  int loglevel;
  char game_id[11];
}configData;


/**
 * Wenn der path ungültig ist, öffne common_config
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

#endif