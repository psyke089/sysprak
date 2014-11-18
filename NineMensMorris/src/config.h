#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <fcntl.h>
#include <unistd.h>
#include <getopt.h>
#include <string.h>

#define MSGLEN 100

typedef struct {
  char hostname[MSGLEN];
  int portnummer;
  char artdesspiels[MSGLEN];
  int loglevel;
}configParams;

configParams readConfig(FILE * file);