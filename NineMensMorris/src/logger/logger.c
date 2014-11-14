//  Created by Tim K. on 12.11.14.

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <strings.h>
#include <errno.h>

#include <time.h>
#include <unistd.h>

#include "../main.h"



//einb. char words[32][64]; // @todo remove wenn eingebunden mit pc


int loglevel = 1;   // @todo aus struct von config datei auslesen
//  loglevel = conf.loglevel;        // ""
//  ll = 0 : alles auf console, keine logdatei
//  ll = 1 : alles auf console, log v. allem in logdatei
//  ll = 2 : error in konsole,  log v. allem in logdatei
//  ll = 3 : console nur nach prolog, alles in logfile



// FILE *logfile = fopen("./logfile.txt", "r");n //fehler, geht nur in main oder sonst fct.

/**
 * returnt die aktuelle Zeit als string
 * @parblock
 * @param p
 *              s für short         Bsp: 14-11-13+03:02:11
 *              l für längeres      Bsp: Wed Nov 12 22:18:23 2014\n
 *              n für kA            Bsp: @todo
 *              m für short w/ ms   Bsp: 14-11-13+03:02:11:124 //! @todo ms funkioniert noch nicht richtig
 *              d 
 *              m nur Uhrzeit       Bsp: 03:02:11
 * @endparblock
 */
// malloc sizeof(char) //   brauch ich doch nicht??
char* getTimeAsString (char p){
    //  time_t iTime = time(NULL);
    //  char* pTime = ctime(iTime);
   
    struct tm *timeinfo;  //pointer auf time-struct
    time_t rawtime;
    //char* timeString;   // pointer auf strng der returnt wird
    static char timeString[100];
    time ( &rawtime );
    timeinfo = localtime ( &rawtime );
    
    if(p == 'l'){
        strcpy (timeString,  asctime(timeinfo));
    }else if(p == 's'){
        strftime (timeString,100,"%y-%m-%d+%H:%M:%S",timeinfo);
    // printf ( "Current local time and date: %s", asctime (timeinfo) );
    }else if(p == 'n'){
       strftime (timeString,100," %I:%M%p.",timeinfo);
    }else if(p == 'd'){
        strftime (timeString,100,"%c",timeinfo);
    }else if(p == 't'){
        strftime (timeString,100,"%T",timeinfo);
    }else if(p == 'm'){
        char clockStr[3];
        
        clock_t uptime = clock() / (CLOCKS_PER_SEC / 1000);
        sprintf(clockStr, "%lu", uptime);
        strftime (timeString,100,"%y-%m-%d+%H:%M:%S:",timeinfo);
        strcat(timeString, clockStr);
        for (int i = 0; i < 800; i++) {
            printf("%s",timeString);
            usleep(250);
        }
        printf("%lu",uptime);
    }
    return timeString;
}

/**
 * Schreibt in das Logfile
 * hängt den string mit zeit an das logfile an
 */
void writeLog(char* input){
    FILE *logfile = fopen("./logfile.txt", "a");
    //fputs("test: schreibe in das ins logfile mit fputs\n", logfile);
    fprintf(logfile, "test: schreibe in das ins logfile mit fprintf %s\n", getTimeAsString('s'));
    fprintf(logfile, "%s %s\n", getTimeAsString('t'), input);
    fclose(logfile);
}

/**
 * initialisiert neuen abschnitt im logfile
 */
void initLogSession(){
    FILE *logfile = fopen("./logfile.txt", "a");
    
    fprintf(logfile,"---------------------- %s ----------------------\n", getTimeAsString('d') );
    
    fclose(logfile);
}



void createLog(){
    FILE *logfile = fopen("./logfile.txt", "w");
    
    fprintf(logfile,"## Logdatei für NMM, erstellt am %s\n ", getTimeAsString('l'));
    printf ("logfile erstellt \n"); //@todo remove

    fclose(logfile);
}


/**
  * prüft ob logfile vorhanden, wenn nicht wird ein neues erstellt
  *
  */
void checkFile() {
    FILE *logfile = fopen("./logfile.txt", "r");
    if(logfile == NULL) {
        /* datei nicht vorhanden, erstelle logdatei log.txt */
        printf ("Logfile nicht vorhanden, gehe zu createLog() \n"); //@todo remove
        //hier datei erstellen [STEHEN GEbLieben]
        fclose(logfile);
        createLog();
    } else {
        fclose(logfile);
        /* logfile vorhanden */
        printf ("logfile vorhanden, gehe weiter \n"); //@todo remove
    }
}

/**
 * schreibt in die konsole, fügt farbe hinzu
 * ohne newline \n
 * @paramblock
 * @param c ist die Farbe
 * r / g / y / b / m / c
 * bei sonstigem char wird einfachso geprintet
 * @endparamblock
 *prntColor
 */
void prntColor(char c, char* input ){
    if (c == 'r') {
        printf(RED);
        printf("%s" ,input);
        printf(RESET);
    }else if (c == 'g') {
        printf(GREEN);
        printf("%s" ,input);
        printf(RESET);
    }else if (c == 'y') {
        printf(YELLOW);
        printf("%s" ,input);
        printf(RESET);
    }else if (c == 'b') {
        printf(BLUE);
        printf("%s" ,input);
        printf(RESET);
    }else if (c == 'm') {
        printf(MAGENTA);
        printf("%s" ,input);
        printf(RESET);
    }else if (c == 'c') {
        printf(CYAN);
        printf("%s" ,input);
        printf(RESET);
    }else {
        printf("%s" ,input);
    }
}



/**
 * Anfang wo alles hineinfällt 
 * \param[in] c farbe für konsole
 *
 * \param[in] t typ des inputs
 * \parblock
 *          e : error(-..) / s : sonstiges / p : nachrichten des prologs
 * \endparblock
 * bei loglevel außerhalb des definierten wird beides gemacht
 */
void logPrnt (char c, char t, char* input){
    ///@todo upddate loglevel hier?
    
    if       (loglevel == 0) {
    // alles auf console, keine logdatei
        prntColor(c, input);
        
    }else if (loglevel == 1){
    // alles auf console, log v. alles in logdatei
        prntColor(c, input);
        writeLog(input);
        
    }
    else if (loglevel == 2){
    // error in konsole,  log v. allem in logdatei
        if (t == 'e') {
            prntColor(c, input);
            writeLog(input);
        }
        else {
            writeLog(input);
        }
    }
    else if (loglevel == 3){
        // console nur nach prolog, alles in logfile
        if (t == 'p') {
            writeLog(input);
        }
        else {
            prntColor(c, input);
            writeLog(input);
        }
    }
    else{
        fprintf(stderr, "WARNING: Kein Loglevel angegeben!\n");
        prntColor(c, input);
        writeLog(input);
    }

}



//FILE *logfile;

/* test readlog zahlenraten
void readLog (){  // nur test @todo remove
    char *temp;
    FILE *file = fopen("listOfTheVeryBest.txt","r");
}
*/

/*
//###### main ########
int main(int argc, const char * argv[]) {
// brauch ich docht nicht hier
//    FILE *logfile = fopen("./logfile.txt", "r");
//    logfile = fopen("./logfile.txt", "r");
    
    checkFile();
    initLogSession();
    
    //printf("hier main logger.c\n");
    
    //printf("\n\nTest Color\n");
   
    fInput('r', 'e', "test der fInput");
    
    
    return 0;
}*/