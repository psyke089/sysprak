#include "config.h"

char delimiter[] = " \n";
char *ptr;
char line[MSGLEN];

conf readconfig(FILE* file){

  conf configtemp;

  while (fgets(line, MSGLEN, file)) {
      ptr = strtok(line, delimiter);
      if(strcmp("hostname",ptr) == 0){
        ptr = strtok(NULL, delimiter);
        ptr = strtok(NULL, delimiter);
        strcpy(configtemp.hostname,ptr);
      }

      if(strcmp("portnummer",ptr) == 0){
        ptr = strtok(NULL, delimiter);
        ptr = strtok(NULL, delimiter);
        configtemp.portnummer = atof(ptr);
      }

      if(strcmp("artdesspiels",ptr) == 0){
        ptr = strtok(NULL, delimiter);
        ptr = strtok(NULL, delimiter);  
        strcpy(configtemp.artdesspiels,ptr);
      }

      if(strcmp("portnummer",ptr) == 0){
        ptr = strtok(NULL, delimiter);
        ptr = strtok(NULL, delimiter);  
        configtemp.loglevel = atof(ptr);
      }

  }

  return configtemp;
	//exit(0);
}

