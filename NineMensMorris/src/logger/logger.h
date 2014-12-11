#ifndef logger_h
#define logger_h

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <strings.h>
#include <errno.h>

#include <time.h>
#include <unistd.h>

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


void setLogLevel(int loglevel);


/**
 * Anfang wo alles hineinfällt 
 * \param[in] c farbe für konsole
 *
 * \param[in] t typ des inputs
 * \parblock
 *          e : error(-..) / 
 *			s : sonstiges / 
 *			p : nachrichten des prologs
 *			t : testoutput : funtioniert wie p
 *  		q: perror
 * \endparblock
 * bei loglevel außerhalb des definierten wird beides gemacht
 * @todo 4: fehler/alles ins log
 *  rest in console
 *
 */
void logPrnt (char c, char t, char* input);

void initLog();

char* getTimeAsString (char p);

void prntColor(char color, char isError, char* input );

#endif