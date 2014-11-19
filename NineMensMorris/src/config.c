#include "config.h"

char delimiter[] = " \n";
char *ptr;
char line[MSGLEN];

configData readConfig(FILE *file){

  configData configTemp;

  while (fgets(line, MSGLEN, file) != NULL) {
      ptr = strtok(line, delimiter);
      if(strcmp("hostname",ptr) == 0){
        ptr = strtok(NULL, delimiter);
        ptr = strtok(NULL, delimiter);
        strcpy(configTemp.hostname,ptr);
      }

      if(strcmp("portnummer",ptr) == 0){
        ptr = strtok(NULL, delimiter);
        ptr = strtok(NULL, delimiter);
        configTemp.portnummer = atof(ptr);
      }

      if(strcmp("artdesspiels",ptr) == 0){
        ptr = strtok(NULL, delimiter);
        ptr = strtok(NULL, delimiter);  
        strcpy(configTemp.artdesspiels,ptr);
      }

      if(strcmp("portnummer",ptr) == 0){
        ptr = strtok(NULL, delimiter);
        ptr = strtok(NULL, delimiter);  
        configTemp.loglevel = atof(ptr);
      }

  }

  return configTemp;
}

