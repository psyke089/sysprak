#include "config.h"

//Delimiter für die Trennbedinung pro Zeile
char *delimiter = " ";
char *ptr;
char line[MSGLEN];

configData readConfig(FILE *file){

  //Temporäres Struct configTemp erzeugen
  configData configTemp;

  //Zeilenweise auslesen und aufsplitten nach Parameter 
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

      if(strcmp("loglevel",ptr) == 0){
        ptr = strtok(NULL, delimiter);
        ptr = strtok(NULL, delimiter);  
        configTemp.loglevel = atof(ptr);
      }
  }

  //Struct zurueckgeben
  return configTemp;
}

