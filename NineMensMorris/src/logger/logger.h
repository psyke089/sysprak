#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <strings.h>
#include <errno.h>

#include <time.h>
#include <unistd.h>

#include "../main.h"  


void logPrnt (char c, char t, char* input);

void initLog();

char* getTimeAsString (char p);

void prntColor(char color, char isError, char* input );

