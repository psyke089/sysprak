//  Created by Tim K. on 12.11.14.

#include "./logger.h"


//einb. char words[32][64]; // @todo remove wenn eingebunden mit pc


int loglevel = 1;   // wird mit setLogLevel nun gesetzt.
// nicht aktuell@todo aus struct von config datei auslesen
//  loglevel = conf.loglevel;        // ""
//  ll = 0 : alles auf console, keine logdatei // evtl sinnvoller/logischer wenn dann kein loglevel
//  ll = 1 : alles auf console, log v. allem in logdatei
//  ll = 2 : error in konsole,  log v. allem in logdatei
//  ll = 3 : console nur nach prolog, alles in logfile




void setLogLevel(int loglvl){

    loglevel = loglvl;

}

// FILE *logfile = fopen("./logfile.txt", "r");n //fehler, geht nur in main oder sonst fct.


/**
 * returnt die aktuelle Zeit als string
 * @parblock
 * @param p
 *              s für short         Bsp: 14-11-13+03:02:11
 *              l für längeres      Bsp: Wed Nov 12 22:18:23 2014\n
 *              o für               Bsp               
 *              n für kA            Bsp: @todo
 *              m für short w/ ms   Bsp: 14-11-13+03:02:11:124 //! @todo ms funkioniert noch nicht richtig
 *              d 
 *              ? nur Uhrzeit       Bsp: 03:02:11
 * @endparblock
 */
// malloc sizeof(char) //   brauch ich doch nicht??
char* getTimeAsString (char p){
    //  time_t iTime = time(NULL);
    //  char* pTime = ctime(iTime);
   
    struct tm *timeinfo;  //pointer auf time-struct
    time_t rawtime;
    //char* timeString;   // pointer auf strng der returnt wird
    static char  timeString[100];
    time ( &rawtime );
    timeinfo = localtime ( &rawtime );
    
    if      (p == 'l'){
        strcpy (timeString,  asctime(timeinfo));
    }else if(p == 's'){
        strftime (timeString,100,"%y-%m-%d+%H:%M:%S",timeinfo);
    // printf ( "Current local time and date: %s", asctime (timeinfo) );
    }else if(p == 'n'){
       strftime (timeString,100," ",timeinfo);
    }else if(p == 'o'){
       strftime (timeString,100,"%F - %T",timeinfo);
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
    //fprintf(logfile, "test: schreibe in das ins logfile mit fprintf %s\n", getTimeAsString('s'));
    fprintf(logfile, "%s %s\n", getTimeAsString('t'), input);
    fclose(logfile);
}

/**
 * initialisiert neuen abschnitt im logfile
 */
void initLogSession(){
    FILE *logfile = fopen("./logfile.txt", "a");
    
    fprintf(logfile,"---------------------- %s ----------------------\n\n", getTimeAsString('o') );
    
    fclose(logfile);
}



void createLog(){
    FILE *logfile = fopen("./logfile.txt", "w");
    
    fprintf(logfile,"## Logdatei für NMM, erstellt am %s\n", getTimeAsString('l'));
    printf (GREEN "\nLogfile erstellt! \n" RESET); //@todo remove

    fclose(logfile);
}


/**
  * prüft ob logfile vorhanden, wenn nicht wird ein neues erstellt
  *
  */
void initLog() {
    //  printf ("hier in initLog() \n"); //@todo remove
    FILE *logfile = fopen("./logfile.txt", "r");
    if(logfile == NULL) {
        /* datei nicht vorhanden, erstelle logdatei log.txt */
        printf (RED "\nLogfile nicht vorhanden, gehe zu createLog() \n" RESET); //@todo remove
        //hier datei erstellen [STEHEN GEbLieben]
       // fclose(logfile);
        createLog();
        initLogSession();
    } else {
        fclose(logfile);
        /* logfile vorhanden */
        printf (GREEN "\nLogfile vorhanden, gehe weiter! \n" RESET); //@todo remove
        initLogSession();
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
void prntColor_legacy(char c, char* input ){
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


void prntColor(char color, char isError, char* input ){
    //char colorString[9]
    switch(color){

        case 'r':
            printf(RED);
            break;
        case 'g':
            printf(GREEN);
            break;
        case 'y':
            printf(YELLOW);
            break;
        case 'b':
            printf(BLUE);
            break;
        case 'm':
            printf(MAGENTA);
            break;
        case 'c':
            printf(CYAN);
            break;
        default:
            break;
            
    }if (isError == 'e') {
        perror(input);
    }else {
        printf("%s" ,input);
    }
    
    printf(RESET);
}


/**
 * Anfang wo alles hineinfällt 
 * \param[in] c farbe für konsole
 *
 * \param[in] t typ des inputs
 * \parblock
 *          e : error(-..) / s : sonstiges / p : nachrichten des prologs
 *  @todo q: perror
 * \endparblock
 * bei loglevel außerhalb des definierten wird beides gemacht
 * @todo 4: fehler/alles ins log
 *  rest in console
 *
 */
void logPrnt (char c, char t, char* input){
    ///@todo upddate loglevel hier?
    
    if       (loglevel == 0) {
    // alles auf console, keine logdatei
        prntColor(c,'0', input);
        
    }else if (loglevel == 1){
    // alles auf console, log v. alles in logdatei
        prntColor(c,'0',input);
        writeLog(input);
        
    }
    else if (loglevel == 2){
    // error in konsole,  log v. allem in logdatei
        if (t == 'e') {
            prntColor(c,'e', input);
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
        else if (t == 't') {
            writeLog(input);
        }
        else if (t == 'e') {
            prntColor(c,'e', input);
            writeLog(input);
        }
        else {
            prntColor(c,'0', input);
            writeLog(input);
        }
        


    }

    else{
        fprintf(stderr, "WARNING: Kein Loglevel angegeben!\n"); //@todo auch über logprnt lösen
        prntColor('r', 'e',input);
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
