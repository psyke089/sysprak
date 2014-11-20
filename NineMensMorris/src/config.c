#include "config.h"
#include "main.h"

//Delimiter für die Trennbedinung pro Zeile
char *delimiter = " ";
char *ptr;
char line[MSGLEN];



FILE* open_common_config(FILE* file){

  if(file == NULL){
     if ((file = fopen("client.conf", "r")) == NULL){
        if ((file = fopen("../client.conf", "r")) == NULL){
            perror ("Couldn't open client.conf");
            exit(EXIT_FAILURE);
        } else {printf(GREEN "\nUsing common client.conf\n" RESET);}
      } else {printf(GREEN "\nUsing common client.conf\n" RESET);}
   } 
  return file;
}


FILE* open_path_config(char *optarg){

  FILE* file = NULL;
  char path[PATHLEN];

   if (strcpy(path,optarg) == NULL){
               perror (RED "\nCouldn't copy the c-Flag to path\n" RESET);
   }
   if ((path[strlen(path) - 1] != 'f') || ((file = fopen(path, "r")) == NULL) ){
        printf (RED "\nPath: %s\n", path);
        perror ("Couldn't open" RESET);
   } 
   else {
    printf (GREEN "\nUsing %s\n" RESET, path);
   }

   return file;
}


void id_valid(char *idFlag){

      if (idFlag != NULL && strlen(idFlag) != 11){
          perror(RED "\nDie Länge der Game-ID muss 11 Zeichen lang sein!\n" RESET);
          printHowToUse();
      }
      if (idFlag == NULL){
          perror(RED "\nDie ID wurde nicht erfolgreich gesetzt!\n" RESET);
          printHowToUse();
      }

}

void config_valid(configData conf_str){

      if (conf_str.hostname == NULL || conf_str.portnummer == 0 || conf_str.artdesspiels == NULL || conf_str.loglevel < 0 || conf_str.loglevel > 3){
          perror(RED "\nDie Parameter in der .conf Datei sind nicht alle richtig angegeben!\n" RESET);
          exit(EXIT_FAILURE);
      }

      if (conf_str.hostname == NULL || conf_str.portnummer == 0 || conf_str.artdesspiels == NULL ){
          printHowToUse();
      }
   
}

void print_config_str(configData conf_str){

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

  //Struct zurueckgeben
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
