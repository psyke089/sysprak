#include "config.h"
#include "logger/logger.h"

FILE* openCommonConfig(FILE* file){

  if(file == NULL){
     if ((file = fopen("client.conf", "r")) == NULL){
        if ((file = fopen("../client.conf", "r")) == NULL){
            logPrnt('r', 'e', "Couldn't open client.conf\n");
            exit(EXIT_FAILURE);
        } else {logPrnt('g', 'p', "\nUsing common ../client.conf\n");}
      } else {logPrnt('g', 'p', "\nUsing common client.conf\n");}
   } 
  return file;
}


FILE* openPathConfig(char *optarg){
  char log_msg[200];
  FILE* file = NULL;
  char path[PATHLEN];

   if (strcpy(path,optarg) == NULL){
               logPrnt('r', 'e', "\nCouldn't copy the c-Flag to path\n");
   }
   if ((path[strlen(path) - 1] != 'f') || ((file = fopen(path, "r")) == NULL) ){
    //asprintf invalid in C99 in mac
    //asprintf(&log_msg, "\nCouldn't open path = ' %s '!\n", path);
    sprintf(log_msg,"\nCouldn't open path = ' %s '!\n", path);
    logPrnt('r', 'e', log_msg);
    } 
   else {
    //asprintf(&log_msg, "\nUsing = ' %s '!\n", path);
    sprintf(log_msg, "\nUsing = ' %s '!\n", path);
    logPrnt('g', 'p', log_msg);
    }

   return file;
}


void configParamValid(configData conf_str){

      if (conf_str.hostname == NULL || conf_str.portnummer == 0 || conf_str.artdesspiels == NULL || conf_str.loglevel < 0 || conf_str.loglevel > 3){
          logPrnt('r', 'e', "\nDie Parameter in der .conf Datei sind nicht alle richtig angegeben!\n");
          exit(EXIT_FAILURE);
      }

      if (conf_str.hostname == NULL || conf_str.portnummer == 0 || conf_str.artdesspiels == NULL ){
          printHowToUse();
      }
   
}

void printConfigString(configData conf_str){

      printf ("host = %s \n"
              "port = %i \n"
              "artds= %s \n"
              "loglvl = %i \n", 
              conf_str.hostname,
              conf_str.portnummer,
              conf_str.artdesspiels,
              conf_str.loglevel);

}

configData readConfig(FILE *file){

  //Delimiter für die Trennbedinung pro Zeile
  char *delimiter = " ";
  char line[MSGLEN];
  char *ptr;
  //Temporäres Struct configTemp erzeugen
  configData configTemp;

  //Zeilenweise auslesen und aufsplitten nach Parameter 
  while (fgets(line, MSGLEN, file) != NULL) {
      ptr = strtok(line, delimiter);
      if(strcmp("hostname",ptr) == 0){
        ptr = strtok(NULL, delimiter);
        ptr = strtok(NULL, delimiter);
        strcpy(configTemp.hostname,ptr);
        configTemp.hostname[strlen(configTemp.hostname)-1] = 0;
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
        configTemp.artdesspiels[strlen(configTemp.artdesspiels)-1] = 0;
      }

      if(strcmp("loglevel",ptr) == 0){
        ptr = strtok(NULL, delimiter);
        ptr = strtok(NULL, delimiter);  
        configTemp.loglevel = atof(ptr);
      }
  }

  fclose(file);

  return configTemp;
}




void printHowToUse (){
  printf("\n"
         "NAME:                                              \n"
         "           client - Unser 'Nine Mens Morris' client\n"
         "OPTIONS:                                           \n"
         "          -i Game-ID (11 Zeichen lang)             \n"
         "          -c Relativer configdateipfad (optional)  \n"
         "\n");
  exit(EXIT_FAILURE);
}
