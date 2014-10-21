#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct {
   int versuche;
   int zuerraten;
} entry;
 
entry highscores[10];

void writeHighscores()
{	
	FILE *scores = fopen("highscores.txt", "w");
	
	printf("write\n");

	if (scores == NULL)
	{
	    printf("Error opening file!\n");
	    exit(1);
	}

	for (int i = 0; i < 10; ++i)
	{
		fprintf(scores, "%i,%i\n", highscores[i].zuerraten, highscores[i].versuche);	
	}

	fclose(scores);
}

void readHighscores()
{	
	int i = 0;
	char *line;
	char *token;
	FILE *scores = fopen("highscores.txt", "r");

	printf("read\n");

	while ( fgets(line, sizeof line, scores) != NULL )
	{
		//highscores[i].versuche = atoi(strsep(line, ","));
		//highscores[i].zuerraten = atoi(strsep(line, ","));
		
		while ((token = strsep(&line, ",")) != NULL)
                   printf("%s\n", token);

		//printf("%s\n", token);
		i++;
	}

	fclose(scores);
}

void zeroHighscores()
{	
	printf("zero\n");
	for (int i = 0; i < 10; ++i)
	{
		highscores[i].versuche = 0;
		highscores[i].zuerraten = 0;
	}
}

int main()
{
   	char str[30];
   	char *ptr;
   	long inputNum;
   	int guesses = 0;
   	int toGuess;
   	int cont = 1;

   	srand(time(NULL));
   	
   	toGuess = rand() % 100;

   	readHighscores();
   	//zeroHighscores();
   	//writeHighscores();

   	printf("=======================\n");

   	exit(0);
   	
   	printf("Auf geht's!\n");

   	while(cont){

   		scanf("%s", &str);
   		
   		if (strcmp(str,"quit") == 0){
   			exit(0);
   		}

   		inputNum = strtol(str, &ptr, 10);
   		
   		if(inputNum == toGuess){
   			printf("great success!!\n");
   			printf("Versuche: %i\n", guesses);
   			printf("noch eine Runde? (j/n)\n");
   			scanf("%s", &str);
   			if (strcmp(str, "n") == 0){
   				cont = 0;
   			}else{
   				guesses = 0;
   				toGuess = rand() % 100;
   				printf("Neue Runde, neues Glück!\n");
   			}
   		}else if( inputNum < toGuess ){
   			printf("Zu erratende Zahl ist größer.\n");
   			guesses++;
   		}else{
   			printf("Zu erratende Zahl ist kleiner.\n");
   			guesses++;
   		}

	}
   	
   	return(0);
}